#######
I've implemented a new method for processing incoming CAN msgs.

The workflow should go like this:
1. CAN Msg arrives
2. interrupt is trigerred
3. the postman thread puts reads the msg and puts in the mailbox
4. the rxcanclient thread checks its mailbox and acts on the canmsg

I've copied the code i used to test my changes here. I didnt have much time to clean it up. I'll also attach the python script
Basically, the script runs for 10 seconds and tries to read as many can msgs as possible. my script sends 1000 msgs and the fw successfuly receives 1000.