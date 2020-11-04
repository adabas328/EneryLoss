This macro is designed to read the data points from the input files for p+p and heavy-ion collisions and calculate the energy loss from it.

The first macro is designed to plot the points from the PPSpecificData and the PbPbSpecificData files to create a pT over Sloss plot.

If you want to place different input files, you made do so but most make sure the nPoints match with however many points are in the file. 

What to specifically put in the command lines is presented below:

> root -l
> .L draw_SLOSS.c
> SLOSS()
> .q




The second macro is created to cross check the data from PHENIX and to make sure that the macro is correctly able to output the same results.

The files needed for this macro is the ppCrossCheckData.txt which is placed in "input file 1" and PbPbCrossCheckData.txt which is placed in "input file 2".

The 26.4 is the thickness scale used for the pp data.

The 1 placed at the end of the command lines switches between 0 and 1. O is for reconstructed jets while 1 is the cross check graph made.




What to specifically put in the command lines is presented below:

> root -l
> .L SlossCrossCheck.c
> SLOSS(input file 1, input file 2,26.4,1)
> .q
