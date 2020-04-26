class Scanner
{
  private:
    int safeTimeBeforeShot = 2000; // Time before Shot - Time for the camera to adjust lightning
    int safeTimeAfterShot = 500;
    
  public:
    int magazineLength = 0;
    ScannerStatus status = STANDBY;
    PCom projector;
    Camera cam;
    LCD* lcd;

    // Constructor
    void init (int magazineLength, LCD* lcd)
    {
      cam.init(2, 3);
      this->lcd = lcd;
      this->magazineLength = magazineLength;
      this->resetRun();
    }

    void prepareRun()
    {
      this->lcd->displayMessage("Prepare");
      this->status = PREPARE_RUN;
      cam.focus();
      projector.resetSystem();
      this->lcd->displayMessage("Ready");
      this->status = READY_TO_START_RUN;
    }

    void executeRun()
    {
      this->lcd->displayMessage("Start Run");
      this->status = EXECUTE_RUN;

      for (int i = 0 ; i < this->magazineLength ; i++)
      {
        Logger::debug("Start Capture Dia Nr. " + String(i+1) + " / " + String(this->magazineLength));
        this->lcd->displayMessage("No " + String(i+1) + " / " + String(this->magazineLength));
        delay(this->safeTimeBeforeShot);
        cam.capture();
        delay(this->safeTimeAfterShot);
        projector.slideForward();

        // Finish
        if (i+1 == this->magazineLength)
        {
          Logger::debug("Rund Finished");
          this->lcd->displayMessage("Fertig!");
          this->status = FINISHED_RUN;
        }

        // Cancel ?
        if (IOElements::getButtonRight())
        {
          this->cancelRun();
          break;
        }
      }
    }

    void cancelRun()
    {
      this->status = CANCELED_RUN;
      this->lcd->displayMessage("Canceled");
      delay(1000);
    }

    void resetRun()
    {
      this->status = STANDBY;
      this->lcd->displayMessage("Standby");
    }
};