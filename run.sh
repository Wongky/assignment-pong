git init
git remote add origin https://github.com/hkust-smartcar/assignment-pong
git pull origin master
git remote rm origin
git remote add origin https://github.com/Wongky/assignment-pong.git
git add *
git commit -a -m "initial commit"
git push --set-upstream origin master