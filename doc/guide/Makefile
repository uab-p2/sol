SITE_DIR = site
MKDOCS = mkdocs

.PHONY: all build serve clean rebuild doctor help campaign_pages sections

all: build

build: campaign_pages

campaign_pages: sections
	$(MKDOCS) build

serve: sections
	$(MKDOCS) serve

sections:
	$(shell python3 ../tool/campaign.py)

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
