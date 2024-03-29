#ifndef IOBSERVER_HH
#define IOBSERVER_HH

class MultilayerNetwork;

class IObserver
{
public:
  IObserver(MultilayerNetwork *multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  virtual ~IObserver(void){};

  virtual void atStart() = 0;
  virtual void atStep() = 0;
  virtual void atFinish() = 0;
  virtual double getResult() = 0;
protected:
  MultilayerNetwork *mMultilayerNetwork;
};

#endif
