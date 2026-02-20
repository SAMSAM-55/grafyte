import base64
import csv
import glob
import hashlib
import os
import zipfile

def b64url_sha256(data: bytes) -> str:
    h = hashlib.sha256(data).digest()
    return base64.urlsafe_b64encode(h).rstrip(b"=").decode("ascii")

def rewrite_path(name: str) -> str:
    # Handle both styles:
    #  1) ".data/purelib/..." at archive root
    #  2) "<dist>.data/purelib/..." (rare but seen in broken wheels)
    if ".data/purelib/" in name:
        # split at the marker and keep what's after it
        return name.split(".data/purelib/", 1)[1]
    if ".data/platlib/" in name:
        return name.split(".data/platlib/", 1)[1]
    return name

def convert_one(whl_path: str) -> str:
    # Keep a valid wheel filename: just add "-fixed" before .whl
    if not whl_path.endswith(".whl"):
        raise ValueError("Not a .whl file")
    out_path = whl_path

    with zipfile.ZipFile(whl_path, "r") as zin:
        # Read all files, applying path + WHEEL edits
        files = {}
        for name in zin.namelist():
            data = zin.read(name)

            new_name = rewrite_path(name)

            if new_name.endswith(".dist-info/WHEEL"):
                text = data.decode("utf-8", errors="replace")
                text = text.replace("Root-Is-Purelib: true", "Root-Is-Purelib: false")
                data = text.encode("utf-8")

            files[new_name] = data

        # Find RECORD path
        record_paths = [n for n in files.keys() if n.endswith(".dist-info/RECORD")]
        if len(record_paths) != 1:
            raise RuntimeError(f"Expected exactly one RECORD, found {record_paths}")
        record_path = record_paths[0]

        # Regenerate RECORD based on final file set
        rows = []
        for name, data in files.items():
            if name == record_path:
                continue
            rows.append([name, f"sha256={b64url_sha256(data)}", str(len(data))])
        # RECORD entry for itself has empty hash/size
        rows.append([record_path, "", ""])

        output = []
        for r in rows:
            output.append(",".join(r))
        files[record_path] = ("\n".join(output) + "\n").encode("utf-8")

    # Write output wheel
    with zipfile.ZipFile(out_path, "w", compression=zipfile.ZIP_DEFLATED) as zout:
        for name, data in files.items():
            zout.writestr(name, data)

    return out_path

if __name__ == "__main__":
    whls = glob.glob("dist/*linux*.whl")
    if not whls:
        raise SystemExit("No wheels found in dist/*.whl")
    for w in whls:
        out = convert_one(w)
        print(f"Converted: {w} -> {out}")