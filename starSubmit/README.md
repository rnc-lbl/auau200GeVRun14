###How to use?    
1- Manually replace '/path/to/your/directory/' in the XML to your directory path (hint, you should change in three places).  
2- Create the following directories in your submission directory:  
mkdir csh list report err log production
3- Make sure the directory listed in the SandBox or links to them are in your submission directory.  
4- To submit a list of files (e.g. fileTest.list):  
star-submit-template -template submitPicoD0AnaMaker.xml -entities listOfFiles=fileTest.list
