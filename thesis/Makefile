TEX = pdflatex
BIBTEX = bibtex
PDF = main.pdf
SRC = *.tex frontmatter/*.tex frontmatter/*.pdf mainmatter/*.tex appendix/*.tex Makefile

default: $(PDF)

$(PDF): $(SRC)
	$(TEX) main.tex
	$(BIBTEX) main
	$(TEX) main.tex
	$(TEX) main.tex

clean:
	rm *.aux *.log *.toc *.lof *.lot *.bbl *.blg frontmatter/*.aux
