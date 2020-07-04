paleta
________________________________________________________________________________

Change terminal colors on the fly independent of terminal emulator.


usage
________________________________________________________________________________

+------------------------------------------------------------------------------+
|                                                                              |
| Setting the palette (input must be one color per line).                      |
|                                                                              |
|   $ paleta < file                                                            |
|   $ cmd | paleta                                                             |
|   $ paleta <<EOF                                                             |
|                                                                              |
|                                                                              |
| Loading the palette (in your .shellrc file) (pick one).                      |
|                                                                              |
|   $ paleta -r                                                                |
|   $ (cat "$HOME/conf/cache/paleta/colors" 2>/dev/null &)                     |
|                                                                              |
+------------------------------------------------------------------------------+
