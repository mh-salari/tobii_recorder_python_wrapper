"""
Filename:     test.py
Author:       Mohammadhossein Salari
Email:        mohammadhossein.salari@gmail.com
Last Modified: 2024/08/19
Description:  Test script for the Tobii Eye Tracker Python wrapper.
              Demonstrates initialization, recording, pausing, and
              data retrieval from the eye tracker.
"""

import tobii_recorder
import time
import pandas as pd


def main():
    license_key = "mh@int.lab"
    print("Initializing EyeTracker...")
    try:
        tracker = tobii_recorder.EyeTracker(2560.0, 1440.0, 0.0, license_key)
    except RuntimeError as e:
        print(f"Failed to initialize EyeTracker: {e}")
        return

    if not tracker.isLicenseValid():
        print("License is invalid or expired. Exiting...")
        return

    print("Starting to record for 1 second...")
    try:
        tracker.start()
        start_time = time.time()
        while time.time() - start_time < 1:
            tracker.update()
    except RuntimeError as e:
        print(f"Error during recording: {e}")
        return

    print("Pausing recording for 1 seconds...")
    tracker.pause()
    time.sleep(1)

    print("Resuming recording for 1 more second...")
    try:
        tracker.start()
        start_time = time.time()
        while time.time() - start_time < 1:
            tracker.update()
    except RuntimeError as e:
        print(f"Error during recording: {e}")
        return

    print("Stopping recording and fetching results...")
    tracker.stop()

    try:
        results = tracker.getFormattedData()
        df = pd.DataFrame(results)
        print(f"Printing {len(results)} gaze points:")
        print(df)
    except RuntimeError as e:
        print(f"Error fetching data: {e}")
        return

    print("Eye tracking session complete.")


if __name__ == "__main__":
    main()
