---
name: charmap-no-emdash
description: Em/en dashes break the script preprocessor in Pelagios — use ASCII hyphen in .string dialogue
metadata:
  type: feedback
---

In `.string` dialogue (data/maps/*/scripts.inc and data/text), never use em dash
(U+2014 `—`) or en dash (U+2013). They are NOT in `charmap.txt` and the script
preproc fails with `error: unknown character U+2014`. Use an ASCII hyphen `-`.

**Why:** Haven Isle's authored scripts were written with em dashes (likely from a
word processor / LLM output) and the first compile failed on 6 scripts.inc files.
The fix is a literal character swap that preserves meaning.

**How to apply:** When a build error reads "unknown character U+XXXX", grep the
HavenIsle (or current island) scripts for non-ASCII bytes:
`LC_ALL=C grep -rnP '[\x80-\xff]' data/maps/<Island>_*/scripts.inc`
then `sed` the em dash to `-`. Note: `é` (POKéMON) and `…` (ellipsis) ARE in the
charmap and are fine — only the dashes break. Future island scripts from the
script-writer agent will likely have the same issue; check proactively before
the first build of each island.
