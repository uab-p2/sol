SITE_DIR = site
SECTION_DIR = sections
MKDOCS = mkdocs

.PHONY: all build serve clean rebuild doctor help campaign_pages sections

all: build

build: campaign_pages

campaign_pages: sections
	$(MKDOCS) build

serve: sections
	$(MKDOCS) serve

sections:
	rm -rf $(SECTION_DIR)/auto/*
	mkdir -p $(SECTION_DIR)/auto
	python3 ../tool/campaign.py
	python3 ../tool/autoindex.py

clean:
	rm -rf $(SECTION_DIR)/auto/*
	mkdir -p $(SECTION_DIR)/auto
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
