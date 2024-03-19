import pytest
import depthai as dai
import numpy as np

def test_nndata_tensor():
  nndata = dai.NNData()

  tensorA = np.random.rand(3,3,3,3)
  tensorB = np.random.randint(2**8, size=(3,3,3,3))
  tensorC = [1,2,3,4,5]
  tensorD = [1.1, 2.1, 3.1, 4.1, 5.1]

  nndata.addTensor("a", tensorA)
  nndata.addTensor("b", tensorB)
  nndata.addTensor("c", tensorC)
  nndata.addTensor("d", tensorD)

  assert(nndata.getTensorDatatype("a") == dai.TensorInfo.DataType.FP16)
  assert(nndata.getTensorDatatype("b") == dai.TensorInfo.DataType.U8F)
  assert(nndata.getTensorDatatype("c") == dai.TensorInfo.DataType.U8F)
  assert(nndata.getTensorDatatype("d") == dai.TensorInfo.DataType.FP16)

  assert(np.allclose(nndata.getTensor("a"), tensorA, atol=0.002))
  assert((nndata.getTensor("b") == tensorB).all())
  assert((nndata.getTensor("c") == tensorC).all())
  assert(np.allclose(nndata.getTensor("d"), tensorD, atol=0.002))