{
      Licensed Materials - Property of IBM
      This product contains "Restricted Materials of IBM"
      5741-A05 (C) Copyright IBM Corp. - 1991, 2004
      All rights reserved.
      US Government Users Restricted Rights -
      Use, duplication or disclosure restricted by GSA ADP Schedule
      Contract with IBM Corp.
      See IBM Copyright Instructions.
}

{
    Change Activity
       VREBA   - IPv6 Stage 1 line item
}

{**********************************************************************}
{*                                                                    *}
{* Memory-to-memory Data Transfer Rate Measurement                    *}
{*                                                                    *}
{* Pseudocode:  Establish access to TCP/IP Services                   *}
{*              Prompt user for operation parameters                  *}
{*              Open a connection (Sender:passive, Receiver:active)   *}
{*              If Sender:                                            *}
{*                Send 5M of data using TcpFSend                      *}
{*                Use GetNextNote to know when Send is complete       *}
{*                Print transfer rate after every 1M of data          *}
{*              else Receiver:                                        *}
{*                Receive 5M of data using TcpFReceive                *}
{*                Use GetNextNote to know when data is delivered      *}
{*                Print transfer rate after every 1M of data          *}
{*              Close connection                                      *}
{*              Use GetNextNote to wait until connection is closed    *}
{*                                                                    *}
{**********************************************************************}
program PSAMPLE;

%include CMALLCL
%include CMINTER
%include CMRESGLB

const
   BUFFERlength = 8192;             { same as MAXdataBUFFERsize      }
   PORTnumber   = 9876;             { constant on both sides         }
   CLOCKunitsPERthousandth = '3E8000'x;

static
   Buffer         : packed array (.1..BUFFERlength.) of char;
   BufferAddress  : Address31Type;
   ConnectionInfo : StatusInfoType;
   Count          : integer;
   DataRate       : real;
   Difference     : TimeStampType;
   HostAddress    : IPAddressType;                            {@VRFBAQP}
   AddrSpec       : IPv6AddrSpecType;                         {@VRFBAQP}
   Lookup         : LookupSetType;                            {@VRFBAQP}
   IbmSeconds     : integer;
   Ignored        : integer;
   Line           : string(80);
   Note           : NotificationInfoType;
   PushFlag       : boolean;        { for TcpFSend                   }
   RealRate       : real;
   ReturnCode     : integer;
   SendFlag       : boolean;        { are we sending or receiving    }
   StartingTime   : TimeStampType;
   Thousandths    : integer;
   TotalBytes     : integer;
   UrgentFlag     : boolean;        { for TcpFSend                   }

 var RoundRealRate : integer;

   {******************************************************************}
   {* Print message, release resources and reset environment         *}
   {******************************************************************}
   procedure Restore ( const Message: string;
                       const ReturnCode: integer );
   begin
      Write(Message);
      if ReturnCode <> OK then
    {*   Write(SayCalRe(ReturnCode));
      Writeln('');                   *}
         Msg1(Output,1, addr(SayCalRe(ReturnCode)) )
      else Msg0(Output,2);

      EndTcpIp;
      Close (Input);
      Close (Output);
   end;


begin
   TermOut (Output);
   TermIn (Input);

   { Establish access to TCP/IP services }
   BeginTcpIp (ReturnCode);
   if ReturnCode <> OK then begin
   {* Writeln('BeginTcpip: ',SayCalRe(ReturnCode)); *}
      Msg1(Output,4, addr(SayCalRe(ReturnCode)) );
      return;
   end;

   { Inform TCPIP which notifications will be handled by the program }
   Handle ((.DATAdelivered, BUFFERspaceAVAILABLE,
             CONNECTIONstateCHANGED, FRECEIVEerror,
             FSendResponse.), ReturnCode);
   if ReturnCode <> OK then begin
      Restore ('Handle: ', ReturnCode);
      return;
   end;

   { Prompt user for operation parameters                            }
{* Writeln('Transfer mode: (Send or Receive)'); *}
   Msg0(Output,5);
   ReadLn (Line);
   if (Substr(Ltrim(Line),1,1) = 's')
   or (Substr(Ltrim(Line),1,1) = 'S') then
      SendFlag := TRUE
   else
      SendFlag := FALSE;

{* Writeln('Host Name or Internet Address :'); *}
   Msg0(Output,6);
   ReadLn (Line);
   Lookup := [IPv4];                                          {@VRFBAQP}
   if not (GetIPAddr(Trim(Ltrim(Line)), HostAddress,          {@VRFBAQP}
                     AddrSpec, Lookup)) then                  {@VRFBAQP}
     begin                                                    {@VRFBAQP}
       Restore ('GetIPAddr failed. ', OK);                    {@VRFBAQP}
       return;                                                {@VRFBAQP}
     end;                                                     {@VRFBAQP}

   { Open a TCP connection: active for Send and passive for Receive }
   {   - Connection value will be returned by TcpIp                 }
   {   - initialize IBM reserved fields:  Security, Compartment     }
   {     and Precedence                                             }
   { for Active open  - set Connection State to TRYINGtoOPEN        }
   {                  - must initialize foreign socket              }
   { for Passive open - set ConnectionState to LISTENING            }
   {                  - may leave foreign socket uninitialized to   }
   {                    accept any open attempt                     }
   with ConnectionInfo do begin
      Connection         := UNSPECIFIEDconnection;
      OpenAttemptTimeout := WAITforever;
      Security           := DEFAULTsecurity;
      Compartment        := DEFAULTcompartment;
      Precedence         := DEFAULTprecedence;
      if SendFlag then begin
         ConnectionState       := TRYINGtoOPEN;
         LocalSocket.Address   := UNSPECIFIEDaddress;
         LocalSocket.Port      := UNSPECIFIEDport;
         ForeignSocket.Address := HostAddress.IPv4Addr;       {@VRFBAQP}
         ForeignSocket.Port    := PORTnumber;
      end
      else begin
         ConnectionState       := LISTENING;
         LocalSocket.Address   := HostAddress.IPv4Addr;       {@VRFBAQP}
         LocalSocket.Port      := PORTnumber;
         ForeignSocket.Address := UNSPECIFIEDaddress;
         ForeignSocket.Port    := UNSPECIFIEDport;
      end;
   end;
   TcpWaitOpen (ConnectionInfo, ReturnCode);
   if ReturnCode <> OK then begin
      Restore ('TcpWaitOpen: ', ReturnCode);
      return;
   end;

   { Initialization }
   BufferAddress := Addr(Buffer(.1.));
   StartingTime  := ClockTime;
   TotalBytes    := 0;
   Count         := 0;
   PushFlag      := false;     { let TcpIp buffer data for efficiency }
   UrgentFlag    := false;               { none of out data is Urgent }

   { Issue first TcpFSend or TcpFReceive }
   if SendFlag then
      TcpFSend (ConnectionInfo.Connection, BufferAddress,
                   BUFFERlength, PushFlag, UrgentFlag, ReturnCode)
   else
      TcpFReceive (ConnectionInfo.Connection, BufferAddress,
                      BUFFERlength, ReturnCode);

   if ReturnCode <> OK then begin
   {* Writeln('TcpSend/Receive: ',SayCalRe(ReturnCode));  *}
      Msg1(Output,7, addr(SayCalRe(ReturnCode)) );
      return;
   end;


   { Repeat until 5M bytes of data have been transferred }
   while (Count < 5) do begin
      { Wait until previous transfer operation is completed }
      GetNextNote(Note, True, ReturnCode);
      if ReturnCode <> OK then begin
         restore('GetNextNote :',ReturnCode);
         return;
      end;

      { Proceed with transfer according to the Notification received   }
      { Notifications Expected :                                       }
      {  DATAdelivered - TcpFReceive function call is now complete     }
      {                - receive buffer contains data                  }
      {  FSENDresponse - TcpFSend function call is now complete        }
      {                - send buffer is now available for use          }
      {  FRECEIVEerror - if there was an error on TcpFReceive function }
      case Note.NotificationTag of
         DATAdelivered:
            begin
               TotalBytes := TotalBytes + Note.BytesDelivered;
               {issue next TcpFReceive                                }
               TcpFReceive (ConnectionInfo.Connection, BufferAddress,
                      BUFFERlength, ReturnCode);
               if ReturnCode <> OK then begin
                  Restore('TcpFReceive: ',Note.SendTurnCode);
                  return;
               end;
            end;
         FSENDresponse:
            begin
               if Note.SendTurnCode <> OK then begin
                  Restore('FSENDresponse: ',Note.SendTurnCode);
                  return;
               end
               else begin
                  {issue next TcpFSend                         }
                  TotalBytes := TotalBytes + BUFFERlength;
                  TcpFSend (ConnectionInfo.Connection, BufferAddress,
                      BUFFERlength, PushFlag, UrgentFlag, ReturnCode);
                  if ReturnCode <> OK then begin
                     Restore('TcpFSend: ',Note.SendTurnCode);
                     return;
                  end;
               end;
            end;
         FRECEIVEerror:
            begin
               Restore('FRECEIVEerror: ', Note.ReceiveTurnCode);
               return;
            end;
         OTHERWISE
            begin
               Restore('UnExpected Notification ',OK);
               return;
            end;
      end; { Case on Note.NotificationTag }


      { is it time to print transfer rate? }
      if TotalBytes < 1048576 then
         continue;

      { Print transfer rate after every 1M bytes of data transferred }
      DoubleSubtract (ClockTime, StartingTime, Difference);
      DoubleDivide (Difference, CLOCKunitsPERthousandth, Thousandths,
                    Ignored);
      RealRate := (TotalBytes/Thousandths) * 1000.0;
   {* Writeln('Transfer Rate ', RealRate:1:0,' Bytes/sec.'); *}
      RoundRealRate := Round(RealRate);
      Msg1(Output,8, addr(RoundRealRate) );

      StartingTime := ClockTime;
      TotalBytes   := 0;
      Count        := Count + 1;
   end; {Loop while Count < 5 }

   { Close TCP connection and wait till partner also drops connection }
   TcpClose (ConnectionInfo.Connection, ReturnCode);
   if ReturnCode <> OK then begin
      Restore ('TcpClose: ', ReturnCode);
      return;
   end;

   { when partner also drops connection, program will receive         }
   {  CONNECTIONstateCHANGED notification with NewState = NONEXISTENT }
   repeat
      GetNextNote (Note, True, ReturnCode);
      if ReturnCode <> OK then begin
         Restore ('GetNextNote: ', ReturnCode);
         return;
      end;
   until (Note.NotificationTag = CONNECTIONstateCHANGED) &
         ((Note.NewState = NONEXISTENT) |
            (Note.NewState = CONNECTIONclosing));

   Restore ('Program terminated successfully. ', OK);
end.
