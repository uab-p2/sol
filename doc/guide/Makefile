SITE_DIR = site
MKDOCS = mkdocs

.PHONY: all build serve clean rebuild doctor help

all: build

build:
	$(MKDOCS) build

serve:
	$(MKDOCS) serve

clean:
	rm -rf $(SITE_DIR)

rebuild: clean build

doctor:
	$(MKDOCS) build --strict

help:
	@echo "Available commands:"
	@echo "  make build    - build static site"
	@echo "  make serve    - start dev server"
	@echo "  make clean    - remove site/"
	@echo "  make rebuild  - clean + build"
	@echo "  make doctor   - strict validation build"
