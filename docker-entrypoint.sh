#!/bin/sh
# Entrypoint for the codebook build container.
#
# Prepends /opt/latexminted-venv/bin to PATH so that `pdflatex -shell-escape`
# can locate the latexminted executable installed by the Dockerfile. Any
# command-line arguments are passed through to exec.

set -e

export PATH="/opt/latexminted-venv/bin:${PATH}"

exec "$@"