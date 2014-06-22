import math
import pdb

FACTOR = 100.0    # scale float values to integer by this factor
B = 3250.0
Rk = Rn =  10.0


def main():

    '''
    Get the Start and end value for Rt
    '''

    Rt_start = math.exp ( ((B * 298.15 / 273.15) - B) / 298.15 ) *10.0  # T0  = 273.15K= 0C
    Rt_end   = math.exp ( ((B * 298.15 / 323.15) - B) / 298.15 ) *10.0  # T50 = 323.15K= 50C
    Step = (Rt_start-Rt_end)/20


    '''
    (Pdb) Rt_start
    27.11958333227012
    (Pdb) Rt_end
    4.302863089406901
    (Pdb)
    '''


    # range needs integer values. below are the shifted float values by a factor of 1000
    # Calculate 20 values
    r = range(int(Rt_start*FACTOR), int(Rt_end*FACTOR),-int(Step*FACTOR))

    print("{"),
    for Rt in r:

        pdb.set_trace()
        T = (B*298.15) / ( B + math.log((Rt/FACTOR)/Rn)*298.15)
        print( "{" + str(int(Rt)) +", "+ str(int(T*FACTOR)) + "}," ), # Genauigkeit von 2 Nachkommastellen


    print("}"),


if __name__ == "__main__":
    main()
