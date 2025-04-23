# For all conflicted files
for file in $(git diff --name-only --diff-filter=U); do
  echo "Resolving: $file"
  git checkout --theirs "$file"
  git add "$file"
done