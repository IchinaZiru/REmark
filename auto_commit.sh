#!/bin/bash

# ステージングをすべてクリア
git reset

# 変更されたdata配下のファイルを取得
files=$(git ls-files --modified --others --exclude-standard | grep '^outputs/')
# 注意: このスクリプトは 全てのディレクトリ内の変更ファイルを対象としています。
#files=$(git ls-files --modified --others --exclude-standard)

# 1ファイルずつ add → commit
for file in $files
do
  echo "Committing $file ..."
  git add "$file"
  git commit -m "outputs data"
done

echo "✔ All changes committed successfully."
