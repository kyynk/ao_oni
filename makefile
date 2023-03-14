ok:
	git add .
	git commit -m $(m)
	git checkout main
	git merge dev
	git push origin main:$(b)