SITE_DIR = site
SECTION_DIR = sections
MKDOCS = mkdocs

.PHONY: all build serve clean rebuild doctor help campaign_pages sections assets

all: build

build: campaign_pages

campaign_pages: sections assets
	$(MKDOCS) build

serve: sections assets
	$(MKDOCS) serve

sections:
	$(shell python3 ../tool/campaign.py)

assets:
	rm -rf $(SECTION_DIR)/asset/img/import && mkdir -p $(SECTION_DIR)/asset/img/import && cp -r ../img/*png $(SECTION_DIR)/asset/img/import

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
