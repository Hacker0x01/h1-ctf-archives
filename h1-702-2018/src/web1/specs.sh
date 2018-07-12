#!/bin/bash

# check if authentication for user ID 2 with signature works
output=$(curl \
  -H 'Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6Mn0.t4M7We66pxjMgRNGg1RvOmWT6rLtA8ZwJeNP-S8pVak' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=getNotesMetadata)

# check if output contains epoch key
if [[ $output != *epochs* ]]; then
  echo FAILED
fi

# check if authentication for user ID 2 with incorrect signature fails
output=$(curl \
  -H 'Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6Mn0.YQ==' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=getNotesMetadata)

# check if output contains epoch key
if [[ $output = *epochs* ]]; then
  echo FAILED
fi

# check if authentication for user ID 2 with none algorithm works
output=$(curl \
  -H 'Authorization: ew0KICAidHlwIjogIkpXVCIsDQogICJhbGciOiAibm9uZSINCn0=.eyJpZCI6Mn0=.YQ==' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=getNotesMetadata)

# check if output contains epoch key
if [[ $output != *epochs* ]]; then
  echo FAILED
fi

# check if authentication for user ID 1 with none algorithm works
output=$(curl \
  -H 'Authorization: ew0KICAidHlwIjogIkpXVCIsDQogICJhbGciOiAibm9uZSINCn0=.eyJpZCI6MX0=.YQ==' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=getNotesMetadata)

# check if output contains epoch key
if [[ $output != *epochs* ]]; then
  echo FAILED
fi

# check if authentication for user ID 3 with none algorithm fails
output=$(curl \
  -H 'Authorization: ew0KICAidHlwIjogIkpXVCIsDQogICJhbGciOiAibm9uZSINCn0=.eyJpZCI6M30=.YQ==' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=getNotesMetadata)

# check if output contains epoch key
if [[ $output = *epochs* ]]; then
  echo FAILED
fi

# check if resetNote for user ID 1 works
output=$(curl \
  -X POST \
  -H 'Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6Mn0.t4M7We66pxjMgRNGg1RvOmWT6rLtA8ZwJeNP-S8pVak' \
  -H 'Accept: application/notes.api.v1+json' \
  http://159.203.178.9/rpc.php?method=resetNotes)

# check if output contains epoch key
if [[ $output != *reset*:true* ]]; then
  echo FAILED
fi

# check if createNote for user ID 1 works
output=$(curl \
  -X POST \
  -H 'Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6Mn0.t4M7We66pxjMgRNGg1RvOmWT6rLtA8ZwJeNP-S8pVak' \
  -H 'Accept: application/notes.api.v1+json' \
  -H 'Content-Type: application/json' \
  -d '{"note":"A"}' \
  http://159.203.178.9/rpc.php?method=createNote)

# check if output contains epoch key
if [[ $output != *url*/rpc.php* ]]; then
  echo FAILED
fi
