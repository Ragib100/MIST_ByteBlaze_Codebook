# syntax=docker/dockerfile:1.7
#
# Reproducible build for the MIST Titan Codebook PDF.
#
# The official texlive/texlive:latest-full image ships TeX Live 2026 with
# minted.sty, which requires latexminted >= 0.7.0 (the executable that
# replaces Pygments calls inside pdflatex). latexminted 0.7.1 on PyPI works
# out of the box on Python 3.14 (the ArgParser Python 3.14 fix from minted
# issues #463/#464 is already included upstream).
#
# The build entrypoint runs `python3 generate_pdf.py` by default. Source
# directories (code/, math/, images/) are bind-mounted at run time via
# docker-compose.yml so contributors can edit code without rebuilding.
#
# NOTE: if you ever switch the base image to a TeX Live release that ships
# minted.sty < 3.8.0, you'll need to pin latexminted==0.6.0 instead and
# re-enable the patch step below (see docker/patches/cmdline.py.patch).

FROM texlive/texlive:latest-full

# System dependencies needed by the build script.
USER root
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        python3 \
        python3-pip \
        python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Working directory inside the image. The host bind-mounts code/, math/, and
# images/ onto /work/{code,math,images} at run time.
WORKDIR /work

# Create an isolated virtualenv for the latexminted Python executable. We use
# an explicit location (/opt/latexminted-venv) rather than the project-local
# .venv-latexminted/ so the image is self-contained.
RUN python3 -m venv /opt/latexminted-venv \
    && /opt/latexminted-venv/bin/pip install --no-cache-dir \
        'latexminted==0.7.1'

# Sanity-check the executable runs.
RUN /opt/latexminted-venv/bin/latexminted --version

# Copy the build script and main TeX source. Other inputs (code/, math/,
# images/) are bind-mounted at run time.
COPY generate_pdf.py /work/generate_pdf.py
COPY notebook.tex    /work/notebook.tex

# Entrypoint prepends the latexminted venv to PATH and execs the command.
# Default command builds the PDF; pass e.g. `bash` to debug inside the image.
COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["python3", "generate_pdf.py"]
