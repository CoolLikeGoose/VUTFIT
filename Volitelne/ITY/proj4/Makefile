all: compile

compile: proj4.tex refs.bib
	pdflatex proj4.tex
	bibtex proj4
	pdflatex proj4.tex
	pdflatex proj4.tex

clean:
	rm -f proj4.aux proj4.bbl proj4.bcf proj4.blg proj4.log proj4.out proj4.run.xml

cleanall: clean
	rm -f proj4.pdf
