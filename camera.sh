#! /bin/bash

DATE=$(date +"%Y-%m-%d_%H%M")

raspistill -t 3000 -o $DATE.jpg