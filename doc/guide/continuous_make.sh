#!/bin/bash
cd ../.. && find . \( -name '*.md' -o -name '*.png' -o -name '*.jpg' -o -name '*.jpeg' \) | entr -s 'cd doc && make'
