#!/bin/bash

read -p "Commit's name:    " com
git add s21_decimal.h s21_decimal.c Makefile
git commit -m "$com"
git push origin develop