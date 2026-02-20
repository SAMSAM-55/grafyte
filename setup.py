from setuptools import setup, find_packages
from setuptools.command.bdist_wheel import bdist_wheel as _bdist_wheel
from distutils.util import get_platform


class bdist_wheel_custom(_bdist_wheel):
    def finalize_options(self):
        super().finalize_options()
        self.root_is_pure = False

    def get_tag(self):
        py, abi, plat = super().get_tag()

        # If setuptools still thinks it's "any", force it to the real platform tag.
        if plat == "any":
            plat = get_platform().replace("-", "_").replace(".", "_")
            # On Windows this becomes e.g. "win_amd64"
            # On Linux this becomes e.g. "linux_x86_64"

        return "py3", "none", plat


setup(
    package_dir={"": "src"},
    packages=find_packages(
        "src",
        include=[
            "grafyte",
            "grafyte.*",
            "__grafyte_internal",
            "__grafyte_internal.*",
        ],
    ),
    include_package_data=True,
    package_data={
        "grafyte": ["__init__.pyi", "py.typed", "*.py"],
        "__grafyte_internal": ["*.so", "*.pyd", "*.pyi", "py.typed"],
    },
    cmdclass={"bdist_wheel": bdist_wheel_custom},
)
