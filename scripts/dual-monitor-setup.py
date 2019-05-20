# A simple wrapper for xrandr that handles two displays

#!/usr/bin/python
import subprocess

# Function that sets up the displays
def setup_displays(primary, secondary):
    # answer/input variable
    answer = 0
    # Take input and setup display
    print("1 for extended display\n2 for duplicate display\n3 for primary display only\n4 for secondary display only\n0 to quit")
    try:
        answer = int(input())
    except:
        print("Enter an integer")
        setup_displays(primary, secondary)
    if answer == 0:
        return
    elif answer == 1:
        subprocess.call(["xrandr", "--output", primary, "--primary", "--auto", "--output", secondary, "--auto", "--right-of", primary])
    elif answer == 2:
        subprocess.call(["xrandr", "--output", primary, "--primary", "--auto", "--output", secondary, "--auto", "--same-as", primary])
        return
    elif answer == 3:
        subprocess.call(["xrandr", "--output", secondary, "--off", "--output", primary, "--auto", "--primary"])
        return
    elif answer == 4:
        subprocess.call(["xrandr", "--output", primary, "--off", "--output", secondary, "--auto", "--primary"])
        return
    else:
        print("Select one of the above options only")
        setup_displays(primary, secondary)
    return;

# Function to get list of displays
def get_displays():
    # return variable
    out = list() 
    #run xrandr to get displays and assign output to temporary variable
    tmp = subprocess.check_output("xrandr | grep -w 'connected' | cut -d ' ' -f 1", shell=True)
    tmp = tmp.decode('utf-8')
    # split the string into output (blank line delimited)
    out = tmp.split("\n")
    # remove blank strings from output and return
    out = list(filter(None, out))
    return out

# Main function
def main():
    # variable for list of displays
    displays = list();
    # get the name of the primary and secondary displays
    displays = get_displays()
    if len(displays) == 1:
        print("Second display was not detected. Exiting")
        return
    print("Detected primary display", displays[0], "and secondary display", displays[1])
    # take care of the setup
    setup_displays(displays[0], displays[1])

# call to main
if __name__ == "__main__":
    main()
