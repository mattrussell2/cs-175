#!/bin/bash

COURSE_NUM="175"
LOCAL_FOLDER=`basename "$PWD"`

correct="n"

cp .devcontainer_orig.json devcontainer.json
cp -r .vscode_original ../.vscode

while [[ ${correct} != "y" ]]; do    
    echo "Please enter your utln for homework.cs.tufts.edu"
    read UTLN
    echo "You wrote ${UTLN}"
    echo "Was this correct? (y/n)"
    read correct

    while [[ ${correct} != "y" && ${correct} != "n" ]]; do
        echo "please enter y or n"
        read correct
    done

    if [[ ${correct} == "y" ]]; then
        echo "success! if you entered the wrong info, please run this script again"
    fi
done

sed -i "s/\$\{UTLN\}/${UTLN}/g" devcontainer.json
sed -i "s/\$\{UTLN\}/${UTLN}/g" ../.vscode/sftp.json

sed -i "s/\$\{COURSE_NUM\}/${COURSE_NUM}/g" ../.vscode/sftp.json
sed -i "s/\$\{COURSE_NUM\}/${COURSE_NUM}/g" devcontainer.json

sed -i "s/\$\{LOCAL_FOLDER\}/${LOCAL_FOLDER}/g" devcontainer.json
sed -i "s/\$\{LOCAL_FOLDER\}/${LOCAL_FOLDER}/g" ../.vscode/sftp.json

