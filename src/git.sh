#!/bin/bash

read -p "Commit's name:    " com
git add .
git commit -m "$com"
git push origin develop