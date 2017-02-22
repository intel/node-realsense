# Make sure bikeshed is installed
#  https://github.com/tabatkins/bikeshed/blob/master/docs/install.md

all : object-recognition.html person-tracking.html slam.html

object-recognition.html : bikeshed/object-recognition.bs
	bikeshed spec bikeshed/object-recognition.bs object-recognition.html

person-tracking.html : bikeshed/person-tracking.bs
	bikeshed spec bikeshed/person-tracking.bs person-tracking.html

slam.html : bikeshed/slam.bs
	bikeshed spec bikeshed/slam.bs slam.html
