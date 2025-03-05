#! /usr/bin/python

import subprocess
import glob
import os

bsqt_files = glob.glob('../tests/*.bsqt')

for bsqt_file in bsqt_files:
    txt_file = os.path.splitext(bsqt_file)[0] + '.txt'
    
    if not os.path.exists(txt_file):
        print(f"File {txt_file} not found. Skipping {bsqt_file}.")
        continue

    try:
        output = subprocess.check_output(['./bsqt_tests_bin', bsqt_file], text=True).strip()
    except subprocess.CalledProcessError as e:
        print(f"Error during running {bsqt_file}: {e}")
        continue

    with open(txt_file, 'r') as file:
        correct = file.read().strip()

    if output == correct:
        print(f"test '{os.path.basename(bsqt_file)}' passed")
    else:
        print(f"test '{os.path.basename(bsqt_file)}' failed")
        print("output:")
        print(output)
        print("")

