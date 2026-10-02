# ICPC Team Notebook

## Prerequisites

- [Python 3](https://www.python.org/)
- A LaTeX distribution with `pdflatex` (e.g., TeX Live)

### Install required packages (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y python3 texlive-latex-base texlive-latex-recommended \
    texlive-latex-extra texlive-fonts-recommended texlive-pictures
```

The `minted` package (used for syntax highlighting) requires [Pygments](https://pygments.org/):

```bash
pip install Pygments
```

Or via apt:

```bash
sudo apt-get install -y python3-pygments
```

### Troubleshooting `minted`

minted v3 does the highlighting through a separate `latexminted` executable
rather than calling Pygments directly. If the build fails with

```
! Package minted Error: minted v3+ executable is not installed or is not added to PATH
```

the copy bundled with your TeX distribution is unusable. On Ubuntu 26.04 the
bundled `latexminted` 0.6.0 crashes on Python 3.14 with
`TypeError: ArgParser.__init__() got an unexpected keyword argument 'color'`
(fixed upstream in `latexminted` 0.7.0, minted issues #463/#464).

Create a `.venv-latexminted` next to `generate_pdf.py` and the build script
will put it first on `PATH` automatically:

```bash
python3 -m venv .venv-latexminted
.venv-latexminted/bin/pip install latexminted
```

The `latexminted` version must match your `minted.sty` (`kpsewhich minted.sty`):
`latexminted` 0.7.x requires minted.sty >= 3.8.0, so with the older minted.sty
3.7.0 shipped by Ubuntu 26.04 you need `latexminted==0.6.0` plus the upstream
Python 3.14 fix applied to `ArgParser.__init__` in
`.venv-latexminted/lib/python3.*/site-packages/latexminted/cmdline.py`:

```python
    def __init__(self, *, prog: str, **kwargs):
        kwargs.setdefault('allow_abbrev', False)
        kwargs.setdefault('formatter_class', argparse.RawTextHelpFormatter)
        super().__init__(prog=prog, **kwargs)
```

## Generating the PDF

```bash
python3 generate_pdf.py
```

This script will:
1. Scan the `code/` directory for source files
2. Generate `contents.tex` with all code sections sorted alphabetically
3. Run `pdflatex -shell-escape notebook.tex` three times to properly build the table of contents and references

The output PDF will be `notebook.pdf`.

## Project Structure

```
├── notebook.tex          # Main LaTeX template
├── generate_pdf.py       # PDF generation script
├── contents.tex          # Auto-generated code listings (do not edit manually)
├── math/                 # Math reference sheets (combinatorics, probability, etc.)
└── code/                 # Source code organized by category
    ├── Data Structure/
    ├── Dynamic Programming/
    ├── Game Theory/
    ├── Geometry/
    ├── Graph/
    ├── Misc/
    ├── Number Theory/
    ├── Some Algorithms/
    └── String/
```

## Adding New Code

1. Place your source file in the appropriate `code/<Category>/` directory
2. Supported extensions: `.c`, `.cc`, `.cpp` (C++), `.java` (Java), `.py` (Python). Other files are included as plain text.
3. Run `python3 generate_pdf.py` to regenerate the PDF

## Generating the PDF (with Docker)

If you have [Docker](https://docs.docker.com/get-docker/) and
[Docker Compose](https://docs.docker.com/compose/install/) installed, the PDF
build is fully reproducible on any machine without installing TeX Live or
patching Python packages:

```bash
docker compose build
docker compose run --rm build
```

After the run, **only `notebook.pdf` appears in the project root**. All the
intermediate LaTeX artifacts (`contents.tex`, `notebook.aux`, `notebook.log`,
`notebook.out`, `notebook.toc`, and the `_minted/` highlight cache) live in a
Docker-managed named volume (`mist_titan_codebook_codebook-build`), never on the
host.

> **Note:** the first time you run this on a fresh checkout, create an empty
> `notebook.pdf` so Docker bind-mounts it as a file (rather than creating it
> as a directory, which `pdflatex` cannot write to):
>
> ```bash
> touch notebook.pdf
> docker compose run --rm build
> ```

To clean up the named volume and free disk space:

```bash
docker compose down --volumes
```

If you'd rather drive Docker by hand, the equivalent command is:

```bash
docker build -t mist-codebook .
docker run --rm \
    -v "$(pwd)/code:/work/code" \
    -v "$(pwd)/math:/work/math" \
    -v "$(pwd)/images:/work/images" \
    -v "$(pwd)/notebook.pdf:/work/notebook.pdf" \
    -v codebook-build:/var/build \
    mist-codebook bash -c '
        mkdir -p /var/build
        export OUTPUT_DIRECTORY=/var/build
        python3 /work/generate_pdf.py
        cp /var/build/notebook.pdf /work/notebook.pdf
    '
```

The Docker image bundles:

- The full TeX Live distribution (matching `notebook.tex`'s package list).
- `latexminted==0.7.1` (the version compatible with TeX Live 2026's
  `minted.sty`, which already includes the upstream Python 3.14 fix from
  minted issues #463/#464 — no patching needed in the image).

If you want to build the PDF without Docker, see the original
[Troubleshooting `minted`](#troubleshooting-minted) section above; that
flow requires matching the host's `minted.sty` version against an
appropriate `latexminted` pin. The Dockerfile approach sidesteps that by
shipping both pieces in known-good versions.

## Acknowledgments

The Python script is a fork of the Stanford ICPC team's notebook generator.
