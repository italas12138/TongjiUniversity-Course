import numpy as np
import json
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.optimizers import SGD

# read xList
f=open("xList.txt")
xList=[]
for line in f.readlines():
	xList.append(json.loads(line))
	for i in range(60):
		xList[-1][i][4]=xList[-1][i][4]/100000000.0
f.close()
x_train = np.array(xList).reshape((len(xList),60,5,1))

# read yList
f=open("yList.txt")
yList=[]
for line in f.readlines():
	yList.append(json.loads(line))
f.close()
y_train = keras.utils.to_categorical(np.array(yList), num_classes=2)

model = Sequential()
# input: 60*5 images -> (60, 5) tensors.
# this applies 32 convolution filters of size 3x3 each.

model.add(Conv2D(64, (3, 3), activation='relu', input_shape = (60,5,1),strides=1))
model.add(MaxPooling2D(pool_size=(2, 2),strides=1))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(2, activation='softmax'))

sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])

model.fit(x_train, y_train, batch_size=256, epochs=100, verbose=1, validation_split=0.2)
