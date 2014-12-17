---
Nucleotide-Control-RGB-LEDs
---

An Arduino sketch that blinks RGB LED strips according to a DNA
sequence. Its purpose is to provide an extra dimension to Christmas, and other
decorative, LED ligthing. Therefore, the sktech is preloaded with a
gene sequence from Norway Spruce, *Picea abies*, so that you can put up
a truly *meta* Christmas tree. Or, you can make a Christmas tree shape
from LEDs and have yourself a minimalist *abstract* Christmas.

The code was developed for LED strips with separate R, G, B LEDs,
but it should work just fine with combined RGB LED strips (or single
LEDs), as long as you do the wiring properly. If you run a true RGB LED strip,
where all the LEDs are combined under one lens, it is possible to get better
transitions and more colors with very little code tweaking. The
information on how the code works is contained in the code comments, so be sure to check those out in the Nucleotide-Control-RGB-LEDs.ino file.

Schematic wiring diagrams for the driving circuit are available in the
repository in both .fzz and .PDF formats, with file names
'Nucleotide-Control-RGB-LEDs.fzz', and 'Nucleotide-Control-RGB-LEDs.PDF', respectively. 


##To Do's before next holliday season

* Real-time access to send GenBank accession numbers, so
everyone can see their favourite gene, or genome, in action.

* Probably Raspberry Pi integration.

* Analogue knobs to change the variables controlling LED behaviour, such as
gradient, speed, max_intensity *et.c.*

* A fouth "dimension" of some sort to signal if the current nucleotide
is exon or intron (coding, or non coding).

*If I get a proper RGB strip I'll move on to amino acid (protein) sequences.




=======
