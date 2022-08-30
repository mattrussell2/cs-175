# Introduction
The course setup for `cs-175` will be as follows:
1. You will be working in the `VSCode` IDE.
2. You will be using the `remote-containers` extension in `VSCode`. 
3. You will be using a provided `Docker` container with course files, scripts, etc. so you can easily access and work with the starter code, compile, run, and run `valgrind` on your local system, etc. 
   
## Background
### Docker
`Docker` is a tool which allows for the creation/maintenance
of what are known as `containers`. `Docker containers` are effectively mini-operating systems that can come pre-packaged with software, etc. Such
containers are commonly used for packaging applications with all the stuff they need to run. Rather than have to build a different version of a given piece of software on multiple systems, using `Docker containers`, you can make one build of your software, and then run it anywhere the containers are supported.

### Remote Containers
`VSCode` has an extension named `remote containers`; this allows you to work 'inside' of a Docker container that is on your local desktop. This means that you can have access to a whole other operating system which comes pre-packaged with software. In our case, this means that you can have access to an `Ubuntu` environment with any and all software that's necessary for the course.


# Install Prerequisites
1. [Install Docker Desktop](https://www.docker.com/products/docker-desktop/)
3. [Install VSCode](https://code.visualstudio.com/)
4. [Install the remote containers extension for VSCode](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

## Note for Windows Users
For Windows users, WSL2 is encouraged, however is not required. See the details here on installation as it relates to remote-containers. https://code.visualstudio.com/docs/remote/containers. [Link for WSL2 installation walkthrough](https://docs.microsoft.com/en-us/windows/wsl/install). If you do **not** use WSL2, make sure to right-click on the Docker task bar item, select `Settings` and update `Resources > File Sharing` with any locations your source code is kept. See [tips and tricks for troubleshooting here](https://code.visualstudio.com/docs/remote/troubleshooting#_container-tips).

# Installing the Docker Container
## Download the Build Scripts
Open a terminal in VSCode, and navigate to where you would like to make a `cs-175` folder for your coursework this semester. Then, run the following command in the integrated terminal
```
git clone https://www.github.com/mattrussell2/graphics-docker.git cs-175
````

This will clone the repository (download all of the files required to build the container), and put everything into a folder named `cs-175`. Now, in your terminal, run
```
cd cs-175/.devcontainer # (NOTE: it's .devcontainer, not devcontainer)
chmod +x prep_install
./prep_install
```
Enter your eecs `utln` when prompted. 

## Build the container
Great! Now, we have the blueprint for the container, but we need to build it. In `VSCode`

*  Press `CTRL/CMD+ SHIFT + p` [CTRL on windows, CMD on mac] 
*  Search for `Remote-containers: Open Folder in Container`
*  Select the `cs-175` directory from the steps above
*  The window should refresh - and the docker container will be built. 

## Notes
* If you see an error related to the `Docker daemon`, make sure you have `Docker` running in the background. 
* Be sure to indicate that you trust the author of the container if prompted. 
* Feel free to press the button to view the logs and see what's happening - this step will take about 5-10m. It will only have to happen once. After the container is built, it'll be quick to load.
* In rare cases, your system might hang at the very beginning (on step 2 or 3). If it's doing this, then run the command `rm ~/.docker/config.json` to remove the docker config file, and start over.      

Once the installation is complete, you should see `Dev Container: cs-175` in the lower left corner of the `VSCode` window. Press the `+` symbol on the right-hand side of your terminal window to open a new terminal from within the Container.

## register-ssh-key
Run the command
```
register-ssh-key
```
This will create an ssh-key for you, so you don't need to enter your password to connect to the halligan server. This will make downloading support code, etc. much easier, and will be used to enable automated backups of your work. To verify that your ssh key is registered, you can run `ssh utln@homework.cs.tufts.edu` after the command finishes. It should connect you to the halligan homework server without any prompting for a password. If you have ssh'd, run `exit` to quit from the homework server.

## Automatic Backups
Excellent! Now, from within the development container in `VSCode`:

0. Ensure your current working directory is `/workspaces/cs-175`
1. Press `CTRL/CMD + SHIFT + p`
2. Search for and select `SFTP: config`

You should now see a file `sftp.json`, with various configuration options. You should not have to edit these (and so can close the file), unless you want to change the remote path. Currently, every time that you save a file in your container, it will be automatically uploaded to the homework server under the path `/h/your_utln/cs-175/...`.

To verify that backups are working:

1. Create a file and make some minor edits to it.
2. Save the file - the automatic backup only happens when you save!
3. Run `ssh utln@homework.cs.tufts.edu`, where you replace `utln` with your utln. 
4. run `cd cs-175`
5. run `ls`
6. You should see the file you just created.
7. run `exit` to quit from the homework server.

## Workflow
Okay! For the future, anytime you want to work on `cs-175` stuff:
1. Open `VSCode`
2. Press `CTRL/CMD + SHIFT + p` 
3. Select `Remote-containers: Open Folder in Container`
4. Select the `cs-175` directory. 
5. Open a terminal and work! 

Pro tip: Sometimes `VSCode` will open automatically to your container if it's the last place you were working. If not, after opening `VSCode`, you can usually simply click on the `cs-175 [Dev Container]` link instead of doing steps 2-4 above.

### Notes on the development environment
* When loading into the container in the way described above, you have direct access to the folders/files that live within the `cs-175` folder on your hard drive. So if you create/edit files in the `cs-175` directory in your dev container, then you'll be able to see the updated files in your the folder browser of your operating system 'as normal'. Likewise, if you remove files from the folder browser, the'll disappear from your dev container as well. **From within the container you will not be able to browse through any files on your machine outside of the `cs-175` directory.** The container is meant only for `cs-175` stuff. This is done through what's called a 'bind mount' - if you want more detail (optional), check out [this link](https://docs.docker.com/storage/bind-mounts/). 
* You have `sudo` access in this terminal (no password required). If you need to install any software, etc., run `sudo apt install xxxxxx`. Also, if you'd like to install `VSCode` extensions within the container, feel free to do so!
* If for whatever reason you get 'lost' in the container, your files are located in `/workspaces/cs-175`. When you load into the container, you are not, as is usual on `UNIX` systems, in `~/` - you are in `/workspaces/cs-175`.

# scripts
We have written some scripts for you to be able to interface with the course files and your files on the halligan server. These are all available from within the container after you've installed it. Note! You will **not** be able to run these commands from a terminal ssh'd into the homework server, nor will they work from a 'regular' terminal on your local system. **They will only work from a terminal running in the dev container you've just built.**

## pull-code
The `pull-code` script is used to pull starter code for an assignment from the homework server to your local system. 
## 
```
pull-code ASSIGNMENT_NAME
```
This script will pull the starter code for a given lab/homework/project For instance, running 
```
pull-code lab0
``` 
Will download the lab0 starter code files and place them in a folder named `lab0`. Warning! This command will overwrite any local files with the same name, so if you've already been working on a lab/hw/project that you're going to download starter code for, make sure to make a local backup before running this command!

## pull-backup
pull-backup will pull the most recent backup of your code that's on the halligan homework server for an assignment, and will place the files in a folder named `hwname-backup`. So, if something goes wrong on your local system and you lose your files for an assignment:

1. Open your development container. 
2. ssh to the homework server (step 4 above) and navigate to your files. 
3. Open the files and verify that they have the correct contents. You can use the unix `cat`, `more`, or `less` commands to quickly peek at the files - or you can use `nano`, `emacs`, or `vim` to browse the files in more depth - google how these are used.
4. Once you've verified that the files are what you want, exit from the homework server. 
5. Run the command `pull-backup hwname`.
6. The files will be downloaded into a folder named `hwname-backup`. 

### .snapshot
One of the nice features of the halligan server is the `.snapshot` directory. This directory contains 'snapshots' of all your work at various points in time. To see it

1. ssh to the homework server
2. run `ls .snapshot`

If you run `ls` on any of the directories within `.snapshot`, you will see a 'frozen snapshot' of your halligan work from that point in time. If you need to ever backup work from the `.snapshot` directory

0. load into the dev container
1. ssh to the homework server, and find which files you want in `.snapshot`. 
2. copy the name of the `.snapshot` directory (e.g. `daily.2022-07-03_0010`)
3. exit from the homework server. 
4. run `pull-backup hwname -s snapshot_directory`
5. the files will be downloaded into a folder named `hwname-backup-snapshot_dir_name`

For example if you wanted the hw1 files from the `~/.snapshot/daily.2022-07-03_0010` directory on the hw server, then you'd run
```
pull-backup hw1 -s daily.2022-07-03_0010
```
The relevant homework files from that .snapshot will be copied to your local system in a folder named `hw1-backup-daily.2022-07-03_0010`.


# Changelog
## 7-19-2022
* removed `COPY .` from `Dockerfile` - we have a guranatee that the `.devcontainer` folder/files will be loaded within the container. 

## 7-17-2022
* updated pull-backup to dynamically pull the remote folder from the sftp.json file. 
* updated readme to reflect the need to be in /workspaces/cs-175 before running config for sftp.

<!-- ## jumbotest
This command is to use our in-house unit testing framework. See here for details: https://gitlab.cs.tufts.edu/mrussell/JumboTest -->
