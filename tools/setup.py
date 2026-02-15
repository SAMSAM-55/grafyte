from setuptools import setup, find_packages

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
        "__grafyte_internal": ["*.pyd", "*.pyi", "py.typed"],
    },
)
