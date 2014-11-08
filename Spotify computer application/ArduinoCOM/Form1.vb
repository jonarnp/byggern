Imports System.IO
Imports System.IO.Ports
Imports System.Threading

Public Class Form1
    Dim currentSong As String
    Dim buffer As String
    Delegate Sub myMethodDelegate(ByVal [text] As String)
    Dim hdlD As New myMethodDelegate(AddressOf processCommand)
    Dim WithEvents SerialPort As New IO.Ports.SerialPort
    Dim p() As Process

    Private Sub Form1_Disposed(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Disposed
        If SerialPort.IsOpen() Then
            SerialPort.Close()
        End If
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ni.Visible = False
        GetSerialPortNames()
    End Sub

    Sub GetSerialPortNames()
        For Each sp As String In My.Computer.Ports.SerialPortNames
            lstPorts.Items.Add(sp)
        Next
    End Sub

    Sub SendSerialData(ByVal Port As String, ByVal data As String)
        If (SerialPort.IsOpen) Then
            SerialPort.Write(data)
        Else
            MsgBox("Not connected to Port.")
        End If
    End Sub

    Sub processCommand(ByVal myString As String)
        buffer = buffer + myString
        Dim str As String
        str = buffer
        If InStr(str, "|") Then
            Dim words As String() = str.Split(New Char() {"|"})
            buffer = ""
            Dim word As String
            For Each word In words
                If (word.Length > 0) Then
                    Dim Spotify As New spotify()
                    Select Case word
                        Case "prev"
                            Spotify.PlayPrev()
                            lstConsole.Items.Add("Play previous song.")
                        Case "next"
                            Spotify.PlayNext()
                            lstConsole.Items.Add("Play next song.")
                        Case "pause"
                            Spotify.PlayPause()
                            lstConsole.Items.Add("Spotify paused.")
                        Case Else
                            '  We received an Unknown command. Deal with it.
                            '  lstConsole.Items.Add("Received: " & word)
                    End Select
                End If
            Next
        End If

    End Sub

    Private Sub SerialPort_DataReceived(ByVal sender As Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort.DataReceived
        Dim str As String = SerialPort.ReadExisting()
        Invoke(hdlD, str)
    End Sub
    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Application.Exit()
        If SerialPort.IsOpen Then
            SerialPort.Close()
        End If
    End Sub
    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        checkSongStatus()
    End Sub

    Private Sub checkSongStatus()
        p = Process.GetProcessesByName("Spotify")
        If p.Count > 0 Then
            lblStatus.Text = "Spotify is running."
            Dim Spotify As New spotify()
            If Spotify.Nowplaying() <> currentSong Then
                currentSong = Spotify.Nowplaying()
                If currentSong = "" Or currentSong = "Paused." Then
                    lstConsole.Items.Add("Spotify Paused.")
                Else
                    lstConsole.Items.Add("Song changed to: " & currentSong)
                    If (SerialPort.IsOpen()) Then
                        SerialPort.Write(currentSong & vbCrLf)
                    End If
                End If
            End If
        Else
            lblStatus.Text = "Spotify is NOT running."
        End If
    End Sub
    Private Sub Form1_SizeChanged(ByVal sender As Object, ByVal e As System.EventArgs) _
 Handles MyBase.SizeChanged

        If Me.WindowState = FormWindowState.Minimized Then
            Me.WindowState = FormWindowState.Minimized
            Me.Visible = False
            Me.ni.Visible = True
        End If

    End Sub
    Private Sub ni_Click(ByVal sender As Object, ByVal e As System.EventArgs) _
 Handles ni.Click

        Me.Visible = True
        Me.WindowState = FormWindowState.Normal
        Me.ni.Visible = False

    End Sub

    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConnect.Click
        If lstPorts.SelectedIndex <> -1 Then
            Try
                If SerialPort.IsOpen Then
                    SerialPort.Close()
                    btnConnect.Text = "Connect"
                Else
                    SerialPort.PortName = lstPorts.SelectedItem.ToString
                    SerialPort.BaudRate = 115200
                    SerialPort.DataBits = 8
                    SerialPort.Parity = Parity.None
                    SerialPort.StopBits = StopBits.One
                    SerialPort.Handshake = Handshake.None
                    SerialPort.Encoding = System.Text.Encoding.Default
                    SerialPort.Open()
                    btnConnect.Text = "Disconnect"
                End If
            Catch ex As Exception
                MsgBox(ex.Message)
            End Try
        Else
            MsgBox("Please choose a serial port", vbInformation, "Serial Port")
        End If
    End Sub
End Class