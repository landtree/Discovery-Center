import cv2
import numpy as np

def simulate_color_blindness(frame):
    red, green, blue = frame[:,:,2], frame[:,:,1], frame[:,:,0]
    frame[:,:,2] = 0.1 * red + 0.9 * green  # Alter red channel
    frame[:,:,1] = 0.1 * red + 0.9 * green  # Alter green channel
    frame[:,:,0] = 0.9 * blue + 0.1 * green # Alter blue channel
    return frame

def simulate_tritanopia(frame):
    red, green, blue = frame[:,:,2], frame[:,:,1], frame[:,:,0]
    frame[:,:,2] = red  # Red channel remains
    frame[:,:,1] = green * 0.95 + blue * 0.05  # Adjust green slightly
    frame[:,:,0] = green * 0.7 + blue * 0.3   # Adjust blue significantly
    return frame

def simulate_protanopia(frame):
    # Adjust the red and green channels for protanopia simulation
    red, green, blue = frame[:,:,2], frame[:,:,1], frame[:,:,0]
    frame[:,:,2] = 0.56667 * green + 0.43333 * blue  # Red channel simulated
    frame[:,:,1] = 0.55833 * green + 0.44167 * blue  # Green channel simulated
    frame[:,:,0] = 0.24167 * red + 0.75833 * green + blue * 0  # Blue remains mostly unchanged
    return frame

def simulate_nearsightedness(frame):
    # Apply a Gaussian blur to simulate nearsightedness
    blurred_frame = cv2.GaussianBlur(frame, (21, 21), 0)
    return blurred_frame

def show_webcam_fullscreen():
    try:
        cv2.namedWindow('video', cv2.WND_PROP_FULLSCREEN)
        cv2.setWindowProperty('video', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
        cap = cv2.VideoCapture(0)
    except Exception as e:
        print("Failed to create window or video Capture.")
        print(e)

    if not cap.isOpened():
        print("Cannot open camera")
        exit()

    current_filter = 'Nearsightedness'

    while True:
        try:
            ret, frame = cap.read()
        except:
            print("Failed to get frame")

        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
        
        key = cv2.waitKey(1)
        #clear out string var
        newKey = ""
        
        if(key != -1):
            print("Recieved: " + chr(key))
            newKey = chr(key)
            #force to lowercase to accept any input
            newKey = newKey.lower()
        #only process if a key is pressed & convert rather than check constantly.
            if newKey == "a":
                current_filter = 'Grayscale'
            elif newKey == "s":
                current_filter = 'Nearsightedness'
            elif newKey == "d":
                current_filter = 'Gaussian Blur'
            elif newKey == "f":
                current_filter = 'Color Blindness'
            elif newKey == "g":
                current_filter = 'Tritanopia'
            elif newKey == "h":
                current_filter = 'Protanopia'
            elif newKey == "q":
                break

        # Apply the selected filter
        if current_filter == 'Grayscale':
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)
        elif current_filter == 'Gaussian Blur':
            frame = cv2.GaussianBlur(frame, (13, 13), 0)
        elif current_filter == 'Color Blindness':
            frame = simulate_color_blindness(frame)
        elif current_filter == 'Tritanopia':
            frame = simulate_tritanopia(frame)
        elif current_filter == 'Protanopia':
            frame = simulate_protanopia(frame)
        elif current_filter == 'Nearsightedness':
            frame = simulate_nearsightedness(frame)

        # Resize to fullscreen
        frame = cv2.resize(frame, (1920, 1080))
        
        # Display the filter name in the lower right corner
        cv2.putText(frame, current_filter, (1600, 1060), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)

        cv2.imshow('video', frame)

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    show_webcam_fullscreen()
