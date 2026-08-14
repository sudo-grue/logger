"""Invoke tasks for building and testing logger. A convenience wrapper
around the raw cmake/ctest commands documented in README.md -- see there
for what's actually happening under each of these.
"""

import shutil

from invoke import task

BUILD_DIR = "build/native"


@task
def build(c):
	"""Configure and build."""
	c.run(f"cmake -S . -B {BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug")
	c.run(f"cmake --build {BUILD_DIR}")


@task
def test(c):
	"""Build (if needed) and run ctest."""
	build(c)
	with c.cd(BUILD_DIR):
		c.run("ctest --output-on-failure")


@task
def tidy(c):
	"""Run clang-tidy over the library sources."""
	build(c)
	c.run(f"cmake --build {BUILD_DIR} --target tidy")


@task
def demo(c):
	"""Build and run the dlog_demo executable."""
	build(c)
	c.run(f"./{BUILD_DIR}/dlog_demo")


@task
def clean(c):
	"""Remove the entire build/ directory."""
	shutil.rmtree("build", ignore_errors=True)
