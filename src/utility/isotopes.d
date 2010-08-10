import std.stream;
import std.range;
import std.algorithm;
import std.math;

string header="#Everything after a '#' is a comment. This file was based on data from the GAMMA NMR libraries
#The gyromagnetic ratio is listed in radians per second per tesla
#
#proton number,	mass number,	multplicity,	gyromagnetic ratio";

void main() {
    int[] Spins = [ 2, 3, 2, 2, 1, 1, 3, 4, 5, 4, 7, 4, 1, 2, 3, 2, 1, 
                     6, 1, 2, 1, 4, 1, 4, 6, 6, 2, 2, 4, 4, 4, 4, 4, 8, 
                     8, 6, 8, 13, 8, 4, 6, 2, 8, 4, 4, 4, 6, 4, 4, 10, 4, 
                     2, 4, 4, 6, 4, 10, 2, 6, 10, 6, 6, 10, 6, 6, 2, 6, 2, 
                     2, 2, 2, 10, 10, 2, 2, 6, 8, 2, 2, 6, 2, 4, 8, 4, 4, 
                     11, 8, 6, 8, 8, 8, 8, 6, 6, 4, 4, 4, 6, 6, 8, 8, 2, 
                     2, 6, 8, 15, 8, 10, 8, 2, 6, 6, 2, 4, 4, 4, 2, 4, 2, 
                     4, 2, 2, 2, 10, 10, 4, 6, 4, 4, 8, 2 ];

/*          An array of known isotope atomic numbers (1=H, 6=C, ...)        */

    int[] Numbers = [ 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 5, 5, 6, 6, 
                       7, 7, 8, 8, 8, 9, 10, 10, 10, 11, 12, 13, 14, 15, 
                       16, 17, 17, 19, 19, 20, 21, 22, 22, 23, 23, 24, 25, 26, 
                       27, 28, 29, 29, 30, 31, 31, 31, 32, 33, 34, 35, 37, 37, 
                       38, 39, 40, 41, 42, 42, 43, 44, 44, 45, 46, 47, 47, 48, 
                       48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 54, 54, 55, 56, 
                       56, 57, 57, 59, 60, 60, 62, 62, 63, 63, 64, 64, 65, 66, 
                       66, 67, 68, 69, 70, 70, 71, 71, 72, 72, 73, 74, 75, 75, 
                       76, 76, 77, 77, 78, 79, 80, 80, 81, 81, 82, 83, 85, 89, 
                       90, 91, 91, 92, 0 ];

/*     An array of known isotope atomic masses  (1=1H, 2=2H, 13=13C, ...)   */

    int[] Masses = [ 1, 2, 3, 3, 4, 6, 6, 7, 8, 9, 10, 
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
                      21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 
                      43, 45, 47, 49, 50, 51, 53, 55, 57, 59, 61, 
                      63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 85, 
                      87, 87, 89, 91, 93, 95, 97, 99, 99, 101, 103, 
                      105, 107, 109, 111, 113, 113, 115, 117, 119, 121, 123, 
                      123, 125, 127, 129, 131, 133, 135, 137, 138, 139, 141, 
                      143, 145, 147, 149, 151, 153, 155, 157, 159, 161, 163, 
                      165, 167, 169, 171, 173, 175, 176, 177, 179, 181, 183, 
                      185, 187, 187, 189, 191, 193, 195, 197, 199, 201, 203, 
                      205, 207, 209, 211, 227, 229, 231, 233, 235, 0 ];



/*  An array of known isotope relative frequencies (from 1H @ 400.130 MHz)  */

    double[] RelFreqs = [ 400.130, 61.424, 426.791, 304.811, double.nan, double.nan, 
                           58.883, 155.503, double.nan, 56.226, 42.990, 128.330, 
                           double.nan, 100.613, 28.913, -40.561, double.nan, 54.242, 
                           double.nan, 376.498, double.nan, 31.586, double.nan, 105.842, 
                           24.496, 104.262, 79.494, 161.977, 30.714, 39.205, 
                           32.635, 18.670, 10.247, 26.925, 97.200, 22.563, 
                           22.557, 39.893, 105.246, 22.615, 99.012, 12.956, 
                           94.939, 35.756, 106.146, 113.705, 25.036, 96.035, 
                           122.028, 13.957, 68.514, 76.313, 100.249, 108.063, 
                           38.633, 130.927, 17.342, 19.606, 37.196, 97.936, 
                           26.076, 26.625, 90.061, 18.462, 20.691, 12.744, 
                           18.310, 16.197, 18.622, 84.832, 88.741, 87.492, 
                           87.680, 142.574, 149.212, 95.755, 51.853, 104.714, 
                           126.241, 80.062, 110.668, 32.811, 52.481, 39.749, 
                           44.466, 52.793, 56.522, 117.202, 21.755, 13.384, 
                           16.517, 13.160, 99.236, 43.818, 15.281, 19.102, 
                           90.741, 13.180, 18.338, 82.079, 11.564, 33.095, 
                           70.475, 19.414, 45.643, 31.722, 12.484, 7.478, 
                           47.976, 16.669, 90.129, 91.038, 9.131, 31.070, 
                           6.874, 7.486, 85.876, 6.918, 71.667, 26.457, 
                           228.970, 231.219, 83.459, 64.297, double.nan, double.nan, 
                           double.nan, double.nan, double.nan, 7.162, -263376.60322 ];
   string[] Names = ["Electron",
                     "Hydrogen",                                                                                                                                                                                                      "Helium", 
                     "Lithium","Beryllium",                                                                                                                             "Boron",    "Carbon",   "Nitrogen",   "Oxygen",   "Fluorine", "Neon", 
                     "Sodium", "Magnesium",                                                                                                                             "Aluminum", "Silicon",  "Phosphorus", "Sulfur",   "Chlorine", "Argon",
                     "Potassium","Calcium","Scandium",   "Titanium", "Vanadium","Chromium",   "Manganese",  "Iron",     "Cobalt", "Nickel",     "Copper",  "Zinc",      "Gallium",  "Germanium","Arsenic",    "Selenium", "Bromine",  "Krypton",
                     "Rubidium","Strontium","Yttrium",  "Zirconium", "Niobium", "Molybdenum", "Technetium", "Ruthenium","Rhodium","Palladium",  "Silver",  "Cadmium",   "Indium",   "Tin",      "Antimony",   "Tellurium","Iodine",   "Xenon", 
                     "Cesium",   "Barium", 
                         "Lanthanum",  "Cerium","Praseodymium", "Neodymium","Promethium", "Samarium","Europium",   "Gadolinium", "Terbium",  "Dysprosium","Holmium","Erbium",     "Thulium",  "Ytterbium", "Lutetium",
                                                         "Hafnium",  "Tantalum","Tungsten",   "Rhenium",    "Osmium",   "Iridium","Platinum",    "Gold",   "Mercury",   "Thallium", "Lead",     "Bismuth",    "Polonium",  "Astatine","Radon",
                     "Francium","Radium",
                         "Actinium","Thorium","Protactinium", "Uranium" ];

   
   double proton_magnetic_ratio = 42.58;//MHz/T
   double field=RelFreqs[0]/proton_magnetic_ratio;

    auto fout = new File("../data/isotopes.dat",FileMode.OutNew);
    fout.writefln(header);
    //Sort the arrays
    sort!("a.at!(0) < b.at!(0)")(zip(Numbers,Masses,Spins,RelFreqs));
    int currentProtonNumber=-1;
    foreach(isotope;zip(Numbers,Masses,Spins,RelFreqs)) {
        int protonNumber = isotope.at!(0);
        int massNumber = isotope.at!(1);
        int multiplcity = isotope.at!(2);
        double relFreq = isotope.at!(3);
        double gyro_ratio=2*PI*relFreq/field;

        assert(massNumber >= protonNumber,"Mass number was less than the proton number");

        if(protonNumber != currentProtonNumber) {
            currentProtonNumber=protonNumber;
            if(protonNumber<Names.length) {
                fout.writefln("#",Names[protonNumber]);
            } else {
                fout.writefln("#","Unknown");
            }
        } 
        if(!isNaN(gyro_ratio)) {
            fout.writefln(protonNumber,"        ",massNumber,"        ",multiplcity,"        ",gyro_ratio); 
        } else if(multiplcity == 1) {
            fout.writefln(protonNumber,"        ",massNumber,"        ",multiplcity,"        ",0); 
        }
    }

}


