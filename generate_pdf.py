#!/usr/bin/python
import os
import subprocess
import shutil

code_dir = "code"
def get_sections():
    sections = []
    for (root, dirs, files) in os.walk(code_dir):
        subsections = []
        section_name = os.path.basename(root)
        sections.append((section_name, subsections))
        for file_name in sorted(files):
            subsection_name = os.path.splitext(file_name)[0]
            if subsection_name[0] == '.':   #Skipping hidden files in unix
                continue
            print(subsection_name)
            relative_path = os.path.join(root, file_name)
            number_of_lines = len(open(relative_path).readlines())
            # Use md5sum for hash calculation
            try:
                hash_value = str(subprocess.check_output(["md5sum", relative_path], stderr=subprocess.DEVNULL)).split()[0][2:-1][:8]
            except:
                hash_value = "########"
            subsections.append((relative_path, subsection_name, number_of_lines, hash_value))
    # Sort sections by section name (excluding the first empty entry)
    return sorted(sections[1:], key=lambda x: x[0])

def get_style(filename):
    ext = filename.lower().split('.')[-1]
    if ext in ['c', 'cc', 'cpp']:
        return 'c++'
    elif ext in ['java']:
        return 'java'
    elif ext in ['py']:
        return 'python'
    else:
        return 'text'

def escape_path(path):
    """Wrap a file path in \\detokenize{} if it contains spaces or other
    characters that LaTeX's \\input cannot parse natively. minted's
    \\inputminted uses \\input under the hood, so spaces in the path
    terminate the argument. \\detokenize converts the spaces to
    \\char\"20 tokens which \\input handles correctly."""
    if ' ' in path:
        return '\\detokenize{%s}' % path
    return path

def texify(s):
    """Escape special LaTeX characters in strings"""
    # Order matters - backslash must be first
    s = s.replace('\\', '\\textbackslash{}')
    s = s.replace('&', '\\&')
    s = s.replace('%', '\\%')
    s = s.replace('$', '\\$')
    s = s.replace('#', '\\#')
    s = s.replace('_', '\\_')
    s = s.replace('{', '\\{')
    s = s.replace('}', '\\}')
    s = s.replace('~', '\\textasciitilde{}')
    s = s.replace('^', '\\textasciicircum{}')
    return s

def get_tex(sections):
    tex = ''
    for (section_name, subsections) in sections:
        tex += '\\section{%s}\n' % texify(section_name)
        for (relative_path, subsection_name, number_of_lines, hash_value) in subsections:
            tex += '\\subsection{\\small %s  \\scriptsize [%s lines] - %s}\n' % (texify(subsection_name), number_of_lines, hash_value)
            # minted v3 does not strip surrounding quotes from the file
            # argument, so the path is passed bare. Paths containing spaces
            # are wrapped in \detokenize so LaTeX's underlying \input can
            # parse them (it would otherwise treat the space as the end of
            # the filename).
            tex += '\\inputminted{%s}{%s}\n' % (get_style(relative_path), escape_path(relative_path))
        tex += '\n'
    return tex

def get_env():
    """Environment for pdflatex, preferring a local latexminted if present.

    minted v3 shells out to a `latexminted` executable. When the one bundled
    with the TeX distribution is unusable, `.venv-latexminted` next to this
    script is put first on PATH so its copy is picked up instead. See README.
    """
    env = os.environ.copy()
    venv_bin = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            ".venv-latexminted", "bin")
    if os.path.isdir(venv_bin):
        env["PATH"] = venv_bin + os.pathsep + env.get("PATH", "")
    return env

if __name__ == "__main__":
    sections = get_sections()
    tex = get_tex(sections)
    with open('contents.tex', 'w') as f:
        f.write(tex)
    
    # Run LaTeX multiple times to generate table of contents properly
    print("Running LaTeX compilation...")
    
    env = get_env()
    # nonstopmode keeps a LaTeX error from blocking the build on stdin.
    # OUTPUT_DIRECTORY, when set, sends .aux/.log/.out/.toc and the final
    # .pdf into a separate directory (used by the Docker compose setup
    # to keep intermediate artifacts out of the host filesystem).
    output_directory = os.environ.get("OUTPUT_DIRECTORY")
    pdflatex_options = ["pdflatex", "-shell-escape", "-interaction=nonstopmode"]
    if output_directory:
        pdflatex_options += [f"-output-directory={output_directory}"]
    pdflatex_options.append("notebook.tex")
    
    # First run - generates content
    subprocess.call(pdflatex_options, env=env)
    
    # Second run - generates table of contents
    subprocess.call(pdflatex_options, env=env)
    
    # Third run - fixes references and page numbers
    status = subprocess.call(pdflatex_options, env=env)
    
    if status != 0:
        print("PDF generation FAILED - see notebook.log")
        raise SystemExit(status)
    print("PDF generation complete!")

