#!/bin/bash

# run as
# make_icons-mediatomb.sh mylogo.jpg
# Will generate all the required mediatomb icons

src="${1}"


convert "${src}" -filter Lanczos -resize 32x32\! -background none -flatten bmp:"mt-icon32.bmp"
convert "${src}" -filter Lanczos -resize 32x32\! -background transparent -flatten png:"mt-icon32.png"
convert "${src}" -filter Lanczos -resize 32x32\! -background white -flatten jpg:"mt-icon32.jpg"

convert "${src}" -filter Lanczos -resize 48x48\! -background white -flatten bmp:"mt-icon48.bmp"
convert "${src}" -filter Lanczos -resize 48x48\! -background transparent -flatten png:"mt-icon48.png"
convert "${src}" -filter Lanczos -resize 48x48\! -background white -flatten jpg:"mt-icon48.jpg"

convert "${src}" -filter Lanczos -resize 120x120\! -background white -flatten bmp:"mt-icon120.bmp"
convert "${src}" -filter Lanczos -resize 120x120\! -background transparent -flatten png:"mt-icon120.png"
convert "${src}" -filter Lanczos -resize 120x120\! -background white -flatten jpg:"mt-icon120.jpg"

convert "${src}" -filter Lanczos -resize 260x260\! -background white -flatten bmp:"mt-icon260.bmp"
convert "${src}" -filter Lanczos -resize 260x260\! -background transparent -flatten png:"mt-icon260.png"
convert "${src}" -filter Lanczos -resize 260x260\! -background white -flatten jpg:"mt-icon260.jpg"
