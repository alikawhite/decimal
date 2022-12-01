#!/bin/bash

read -p "Commit's name:    " com
git add .
# name = "\'$"
git commit -m "$com"
git push origin develop