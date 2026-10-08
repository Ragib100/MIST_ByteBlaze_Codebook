# ICPC Team Notebook

## Configure your team

The team name, institution name, and team members are **not** hardcoded in
`notebook.tex`. They are read from a `.env` file and appear in the PDF's page
header, title, and author line.

1. Copy the example file:

```bash
cp .env.example .env
```

2. Edit `.env` with your own values:

```env
TEAM_NAME=TEAM_NAME
INSTITUTION_NAME=INSTITUTION_NAME
TEAM_MEMBERS=Team member 1, Team member 2, Team member 3
```

- `TEAM_NAME`: shown in the header and the title (`<TEAM_NAME> Team Notebook`).
- `INSTITUTION_NAME`: shown in the header next to the team name.
- `TEAM_MEMBERS`: comma-separated list of names, shown under the title.
- Special LaTeX characters such as `_` and `&` are escaped automatically, so
  write `MIST_Titan`, not `MIST\_Titan`.
- All three values are required. The build stops with an error if one is missing.
- `.env` is read each time you run the build, so changing it does **not**
  require rebuilding the Docker image.

## Generating the PDF (with Docker)

> **No Docker installed?** Download and install [Docker Desktop](https://docs.docker.com/get-docker/) for your platform (Windows, macOS, or Linux). Docker Compose is included with Docker Desktop. On Linux you can also install [Docker Engine](https://docs.docker.com/engine/install/) and [Docker Compose](https://docs.docker.com/compose/install/) separately.

1. Pull the TeX Live Docker image:

```bash
docker pull texlive/texlive:latest
```

2. Make sure you have created your `.env` file (see
   [Configure your team](#configure-your-team) above).

3. If you have [Docker](https://docs.docker.com/get-docker/) and
   [Docker Compose](https://docs.docker.com/compose/install/) installed, the PDF
   build is fully reproducible on any machine without installing TeX Live or
   patching Python packages:

```bash
docker compose build
docker compose run --rm build
```

After the run, **only `notebook.pdf` appears in the project root**. All the
intermediate LaTeX artifacts (`contents.tex`, `config.tex`, `notebook.aux`,
`notebook.log`, `notebook.out`, `notebook.toc`, and the `_minted/` highlight
cache) live in a Docker-managed named volume
(`mist_titan_codebook_codebook-build`), never on the host.

> **Note:** if you delete the notebook.pdf, create an empty
> `notebook.pdf` so Docker bind-mounts it as a file (rather than creating it
> as a directory, which `pdflatex` cannot write to):
>
> ```bash
> touch notebook.pdf
> docker compose run --rm build
> ```

> **Note:** if you edit `generate_pdf.py` or `notebook.tex`, run
> `docker compose build` again, because these two files are copied into the
> image. Changes to `code/`, `math/`, `images/`, and `.env` do not need a rebuild.

To clean up the named volume and free disk space:

```bash
docker compose down --volumes
```

If you'd rather drive Docker by hand, the equivalent command is:

```bash
docker build -t mist-codebook .
docker run --rm \
    --env-file .env \
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

## Acknowledgments

The Python script is a fork of the Stanford ICPC team's notebook generator.
