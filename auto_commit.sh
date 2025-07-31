#!/bin/bash

# ステージングをすべてクリア
git reset

# v2 ディレクトリだけの変更ファイルを取得
files=$(git ls-files --modified --others --exclude-standard)

# 1ファイルずつ add → commit
for file in $files
do
  echo "Committing $file ..."
  git add "$file"
  git commit -m "eval data"
done

echo "✅"
