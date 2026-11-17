#!/bin/sh
# Sourced by pre-commit and pre-push.
# Allowed branches: main, develop, and issue branches cut from develop,
# named as GitHub generates them: <issue-number>-<slug> (e.g. 12-add-lexer);
# the slug may keep accents, as GitHub does not strip them.
branch_policy() {
  branch="$(git symbolic-ref --quiet --short HEAD 2>/dev/null)"
  [ -z "$branch" ] && return 0   # detached HEAD (rebase, bisect): nothing to check
  case "$branch" in
    main|develop) return 0 ;;
  esac
  if ! printf '%s\n' "$branch" | grep -Eq '^[0-9]+-[^/[:space:]]+$'; then
    echo "branch-policy: '$branch' is not allowed." >&2
    echo "  use main, develop, or an issue branch cut from develop: <issue-number>-<slug>" >&2
    return 1
  fi
  # A issue branch must descend from develop once develop exists.
  if git rev-parse --verify --quiet develop >/dev/null &&
     ! git merge-base --is-ancestor develop HEAD 2>/dev/null; then
    echo "branch-policy: '$branch' does not descend from develop; rebase it onto develop." >&2
    return 1
  fi
  return 0
}
branch_policy
