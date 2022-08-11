import numpy
import scipy.io.wavfile


if __name__ == '__main__':

    for i in range(0,2):

        fileName =   'data_' + str(i)           
        constructMatrix = numpy.loadtxt(fileName + '.csv', dtype=numpy.int16,  delimiter=',')
        OutputwaveFilename = fileName + '.wav'
        print('length of the data ',int(len(constructMatrix)), ' data snippet ', constructMatrix[0:int(len(constructMatrix))])
        sample_rate = 16000
        scipy.io.wavfile.write(OutputwaveFilename,sample_rate,constructMatrix[0:int(len(constructMatrix))])
        