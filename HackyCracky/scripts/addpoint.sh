#!/bin/bash

# Prompt for coordinates and store them in $lat and $lng
read -p "Coordinates: " coords
coords=$(echo $coords | tr -d ' ')  # Remove spaces
lat=$(echo $coords | cut -d',' -f1)
lng=$(echo $coords | cut -d',' -f2)

# Prompt for other inputs
read -p "Type: " type
read -p "Sec: " sec
read -p "Diff: " diff
read -p "Access: " access
read -p "Title: " title
read -p "Desc: " desc

# Prompt for tags and store them in an array
echo "Tags (Ctrl+D to finish):"
tags=()
while IFS= read -r tag; do
    tags+=("\"$tag\"")
done

# Join the tags array into a comma-separated string
tags_str=$(IFS=,; echo "${tags[*]}")

if [ -z "$1" ]; then
    echo Hey goofy you forgot the filename, defaulting to new.json
    OUT=new.json
else
    OUT=$1
fi

# Output the JSON object
echo "    {
        \"coords\": [$lat, $lng],
        \"type\": \"$type\", \"sec\": \"$sec\", \"diff\": \"$diff\", \"access\": \"$access\",
        \"title\": \"$title\",
        \"desc\": \"$desc\",
        \"tags\": [$tags_str]
    }" >>$OUT
