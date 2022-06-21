import math
import matplotlib.pyplot as plt

# The changing step of the input angle theta_2 is defined as a global varibale
# Feel free to modify :)
delta_theta_2 = 0.01


# ---------------------------------------------------------------------------------------------------------------------

"""

    The Pseudocode to draw a coupler curve.

    Step1: Input dimensions; r1, r2, r3, r4, r6, sigma,beta, and theta_1.
    where   r1, r2, r3, r4 --> the length of the 4-bar linkage members
            r6             --> the distance from the joint to the coupler point
            beta           --> the angle between the line from the joint to the coupler point 
                               and the coupler
            sigma          --> assembly mode (𝛔 = ±1)
            theta_1        --> the angle between the base member and the ground
    
    Step2: Detect Grashof type.
           Grashof type I : "s + l < p + q" 
           --> Two assembly modes corresponding to the value of 𝛔
           --> The output angle ranges from -PI to PI
           
           Grashof type II: "s + l > p + q" 
           --> No joint can perform a complete revolution
           
    Step3: Compute motion limits on input angle theta_2.
           --> theta_2_min < theta < theta_2_max
            
    Step4: The data procession.
    
    Step5: Draw the coupler curve.  
    
"""


# ---------------------------------------------------------------------------------------------------------------------

"""

    The dimension of the final design presents as below:
    
        r1       = 1.2
        r2       = 2.8
        r3       = 2.3
        r4       = 2.5
        r6       = 1.3
        beta     = 1.57
        theta_1  = 0
        sigma    = -1
    
"""


# ---------------------------------------------------------------------------------------------------------------------

def is_groshof_type_1(r1, r2, r3, r4):
    """
    Determines if a given 4-bar linkage is of Grashof type I
    True is returned, iff linkage is of Grashof type I
    False is returned, o/w

    :param r1: the length of the base link
    :param r2: the length of the input link
    :param r3: the length of the coupler
    :param r4: the length of the output link
    :return: boolean
    """
    ls = [r1, r2, r3, r4]

    ls.sort()

    shortest = ls[0]  # shortest --> the shortest length of the linkage
    longest = ls[3]   # longest --> the longest length of the linkage
    p = ls[1]         # p --> the intermediate member of the linkage
    q = ls[2]         # q --> the intermediate member of the linkage

    if shortest + longest <= p + q:  # Condition of Grashof type I
        return True
    return False


# ---------------------------------------------------------------------------------------------------------------------

def motion_limits(r1, r2, r3, r4):
    """
    Determines the motion limits of input angle theta_2, based on the dimension of the linkage

    :param r1: the length of the base link
    :param r2: the length of the input link
    :param r3: the length of the coupler
    :param r4: the length of the output link
    :return: [theta_2_min, theta_2_max]
    """

    # Initialises the default value of theta_2_min and theta_2_max
    # They are used, iff the motion limits is not computable
    # Draws nothing on the screen
    theta_2_min = 0.0
    theta_2_max = 0.0

    # Iff the linkage is constructable initially
    try:
        # Configuration 1
        if (is_groshof_type_1(r1, r2, r3, r4)) and (abs(r1 - r2) > abs(r3 - r4)):
            theta_2_min = 0.0
            theta_2_max = 2 * math.pi

        # Configuration 2
        elif (not is_groshof_type_1(r1, r2, r3, r4)) and (abs(r1 - r2) > abs(r3 - r4)):
            theta_2_min = (-1) * math.acos((r1 ** 2 + r2 ** 2 - (r3 - r4) ** 2) / (2 * r1 * r2))
            theta_2_max = math.acos((r1 ** 2 + r2 ** 2 - (r3 - r4) ** 2) / (2 * r1 * r2))

        # Configuration 3
        elif (not is_groshof_type_1(r1, r2, r3, r4)) and (abs(r1 - r2) < abs(r3 - r4)):
            theta_2_min = math.acos((r1 ** 2 + r2 ** 2 - (r3 - r4) ** 2) / (2 * r1 * r2))
            theta_2_max = math.acos((r1 ** 2 + r2 ** 2 - (r3 + r4) ** 2) / (2 * r1 * r2))

        # Configuration 4
        elif (is_groshof_type_1(r1, r2, r3, r4)) and (abs(r1 - r2) < abs(r3 - r4)):
            theta_2_min = math.acos((r1 ** 2 + r2 ** 2 - (r3 - r4) ** 2) / (2 * r1 * r2))
            theta_2_max = 2 * math.pi - theta_2_min

        # Iff the motions limits is not computable
        # Prints error message and exits the program
        else:
            print("The motion limits is not computable, please check the length of linkage members.\n")
            quit()

    # Iff the linkage is not constructable initially
    except ValueError:
        print("The linkage cannot be built physically.\n")
        # Simply exits the program
        quit()

    return [theta_2_min, theta_2_max]


# ---------------------------------------------------------------------------------------------------------------------

def process_data(r1, r2, r3, r4, r6, beta, sigma, theta_1):
    """
    Generates the corresponding output angle theta_4, and the coupler angle theta_3,
    based on the changing current input angle theta_2 over the range of motion limits.
    The x coordinate of the coupler point at each step in then appended into list ls_x
    The y coordinate of the coupler point at each step in then appended into list ls_y

    :param r1: the length of the base link
    :param r2: the length of the input link
    :param r3: the length of the coupler
    :param r4: the length of the output link
    :param r6: the distance from the joint to the coupler point
    :param beta: the angle between the line from the joint to the coupler point
                 and the coupler
    :param sigma: assembly mode (𝛔 = ±1)
    :param theta_1: the angle between the base member and the ground
    :return: [ls_x, ls_y]
    """

    ls_x = []
    ls_y = []

    # Gets the motion limits
    theta_2_min = motion_limits(r1, r2, r3, r4)[0]
    theta_2_max = motion_limits(r1, r2, r3, r4)[1]

    # The changing step of the input angle theta_2 is defined as a global varibale
    # Goes back to the top of code to modify it

    # The input angle starts with its minimum value
    current_theta_2 = theta_2_min

    while current_theta_2 <= theta_2_max:

        # Calculates the constants A, B, and C in the quadratic formula
        a = 2 * r1 * r4 * math.cos(theta_1) - 2 * r2 * r4 * math.cos(current_theta_2)
        b = 2 * r1 * r4 * math.sin(theta_1) - 2 * r2 * r4 * math.sin(current_theta_2)
        c = (r1 ** 2 + r2 ** 2 + r4 ** 2 - r3 ** 2
             - 2 * r1 * r2 * (
                     math.cos(theta_1) * math.cos(current_theta_2) + math.sin(theta_1) * math.sin(current_theta_2)))

        # The linkage cannot be built physically
        # Skips the arithmetic
        if (a ** 2 + b ** 2) < c ** 2:
            continue

        # Calculates the corresponding output angle theta_4
        theta_4 = 2 * math.atan2((sigma * math.sqrt(b ** 2 - c ** 2 + a ** 2) - b), (c - a))

        # Calculates the corresponding coupler angle theta_3
        # Similar to ATAN2 in matlab
        y = r1 * math.sin(theta_1) + r4 * math.sin(theta_4) - r2 * math.sin(current_theta_2)
        x = r1 * math.cos(theta_1) + r4 * math.cos(theta_4) - r2 * math.cos(current_theta_2)
        theta_3 = math.atan2(y, x)

        # The x coordinate of joint Q (i.e. the end of the input link)
        # The y coordinate of joint Q (i.e. the end of the input link)
        r_q_x = r2 * math.cos(current_theta_2)
        r_q_y = r2 * math.sin(current_theta_2)

        # The x coordinate of coupler point
        # The y coordinate of coupler point
        r_6_x = r6 * math.cos(theta_3 + beta)
        r_6_y = r6 * math.sin(theta_3 + beta)

        # Calculates the position of coupler point
        r_cp_x = r_q_x + r_6_x
        r_cp_y = r_q_y + r_6_y

        ls_x.append(r_cp_x)
        ls_y.append(r_cp_y)

        # Updates the input angle theta_2
        current_theta_2 += delta_theta_2

    return [ls_x, ls_y]


# ---------------------------------------------------------------------------------------------------------------------

def draw_coupler_curve(ls_x_1, ls_y_1, ls_x_2, ls_y_2):
    """
    Draws the coupler curve on the screen,
    based on the data computed in process_data(args)

    :param ls_x_1:  A list contains the x coordinates of the coupler point at each step, using input assembly mode.
    :param ls_y_1:  A list contains the y coordinates of the coupler point at each step. using input assembly mode.
    :param ls_x_2:  A list contains the x coordinates of the coupler point at each step, using another possible assembly
        mode.
    :param ls_y_2:  A list contains the y coordinates of the coupler point at each step, using another possible assembly
        mode.
    :return: void
    """

    # The customization of the figure
    fig, (ax1, ax2) = plt.subplots(1, 2)
    fig.suptitle("Coupler Curve Generator")

    ax1.spines[["left", "bottom"]].set_position(("data", 0))
    ax1.spines[["top", "right"]].set_visible(False)
    ax1.plot(1, 0, ">k", transform=ax1.get_yaxis_transform(), clip_on=False)
    ax1.plot(0, 1, "^k", transform=ax1.get_xaxis_transform(), clip_on=False)

    ax2.spines[["left", "bottom"]].set_position(("data", 0))
    ax2.spines[["top", "right"]].set_visible(False)
    ax2.plot(1, 0, ">k", transform=ax2.get_yaxis_transform(), clip_on=False)
    ax2.plot(0, 1, "^k", transform=ax2.get_xaxis_transform(), clip_on=False)

    ax1.grid(True, linewidth=2, linestyle='-')
    ax2.grid(True, linewidth=2, linestyle='-')

    ax1.set_title("Assembly mode specified by the User")
    ax2.set_title("Another possible assembly mode")

    # Officially plots the figure
    ax1.scatter(ls_x_1, ls_y_1, color='black')
    ax2.scatter(ls_x_2, ls_y_2, color='blue')

    fig.tight_layout()
    plt.show()


# ---------------------------------------------------------------------------------------------------------------------

def get_length(member_name):
    """
    Gets the User-inputs regarding the length information

    :param member_name: the name of the member
    :return: float (i.e. the length of the required member)
    """
    while True:
        try:
            length = float(input("Please enter the length of " + member_name + ": "))

            # Iff the length of the linkage member is legal
            if length > 0:
                return length

            # Iff the length of the linkage member is illegal
            else:
                # Prints error message
                print("The length of " + member_name + " needs to be greater than 0.\n")
                continue
        except ValueError:
            print("The input is negative or not an numeric expression.\n")
            continue


# ---------------------------------------------------------------------------------------------------------------------

def get_angle(angle_name, description):
    """
    Gets the User-inputs regarding the angle information in radians

    :param description: extra information to specified the requiring angle
    :param angle_name: the name of the angle
    :return: float (i.e. the radians of the requiring angle)
    """

    while True:
        try:
            angle = float(input("Please enter the angle of " + angle_name + description + " in radians: "))
            return angle
        except ValueError:
            print("The input cannot be converted to type float.\n")
            continue


# ---------------------------------------------------------------------------------------------------------------------

def get_assembly_mode():
    """
    Gets the User-input of assembly mode

    :return: int (i.e. the assembly mode of the 4-bar linkage)
    """

    while True:
        try:
            assembly_mode = int(input("Please specify the assembly mode of the system by 1 or -1: "))

            # Iff the assembly mode is legal
            if abs(assembly_mode) == 1:
                return assembly_mode

            # Iff the assembly mode is illegal
            else:
                # Prints error message
                print("The assembly mode is either 1 or -1, please enter again.\n")
                continue
        except ValueError:
            print("Please enter a valid assembly mode.\n")
            continue


# ---------------------------------------------------------------------------------------------------------------------

def __main__():
    """
    The control flow of the coupler curve program
    """
    # Requires the User-inputs
    r1 = get_length("base member")
    r2 = get_length("input member")
    r3 = get_length("coupler member")
    r4 = get_length("output member")
    r6 = get_length("r6")
    beta = get_angle("beta ", "where beta is the angle between the line from the joint to the coupler point and the "
                              "coupler point")
    theta_1 = get_angle("theta_1 ", "where theta_1 is the angle between the base member and the ground")
    sigma = get_assembly_mode()

    # Processing data
    # The assembly mode specified by the User
    ls_x_1 = process_data(r1, r2, r3, r4, r6, beta, sigma, theta_1)[0]
    ls_y_1 = process_data(r1, r2, r3, r4, r6, beta, sigma, theta_1)[1]

    # Processing data
    # Another possible assembly mode
    sigma = sigma * (-1)
    ls_x_2 = process_data(r1, r2, r3, r4, r6, beta, sigma, theta_1)[0]
    ls_y_2 = process_data(r1, r2, r3, r4, r6, beta, sigma, theta_1)[1]

    # Draws the coupler curve
    draw_coupler_curve(ls_x_1, ls_y_1, ls_x_2, ls_y_2)


__main__()
