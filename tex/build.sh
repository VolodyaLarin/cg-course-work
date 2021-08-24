LATEX_ARGS=-shell-escape

echo ${LATEX_ARGS}
rm *.aux *.toc *.out *.log *.bbl *.blg
pdflatex ${LATEX_ARGS} rpz.tex
bibtex rpz
pdflatex ${LATEX_ARGS} rpz.tex 
pdflatex ${LATEX_ARGS} rpz.tex 
