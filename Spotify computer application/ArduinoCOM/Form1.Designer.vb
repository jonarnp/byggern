<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.lstConsole = New System.Windows.Forms.ListBox()
        Me.btnConnect = New System.Windows.Forms.Button()
        Me.lstPorts = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.lblStatus = New System.Windows.Forms.Label()
        Me.ni = New System.Windows.Forms.NotifyIcon(Me.components)
        Me.SuspendLayout()
        '
        'Timer1
        '
        Me.Timer1.Enabled = True
        Me.Timer1.Interval = 2000
        '
        'lstConsole
        '
        Me.lstConsole.BackColor = System.Drawing.Color.White
        Me.lstConsole.FormattingEnabled = True
        Me.lstConsole.Location = New System.Drawing.Point(21, 157)
        Me.lstConsole.Name = "lstConsole"
        Me.lstConsole.Size = New System.Drawing.Size(462, 251)
        Me.lstConsole.TabIndex = 5
        '
        'btnConnect
        '
        Me.btnConnect.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnConnect.Location = New System.Drawing.Point(220, 118)
        Me.btnConnect.Name = "btnConnect"
        Me.btnConnect.Size = New System.Drawing.Size(95, 23)
        Me.btnConnect.TabIndex = 7
        Me.btnConnect.Text = "&Connect"
        Me.btnConnect.UseVisualStyleBackColor = True
        '
        'lstPorts
        '
        Me.lstPorts.FormattingEnabled = True
        Me.lstPorts.Location = New System.Drawing.Point(24, 118)
        Me.lstPorts.Name = "lstPorts"
        Me.lstPorts.Size = New System.Drawing.Size(181, 21)
        Me.lstPorts.TabIndex = 6
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(23, 99)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(89, 13)
        Me.Label1.TabIndex = 8
        Me.Label1.Text = "Select COM Port:"
        '
        'lblStatus
        '
        Me.lblStatus.AutoSize = True
        Me.lblStatus.Location = New System.Drawing.Point(23, 418)
        Me.lblStatus.Name = "lblStatus"
        Me.lblStatus.Size = New System.Drawing.Size(116, 13)
        Me.lblStatus.TabIndex = 9
        Me.lblStatus.Text = "Spotify is NOT running."
        '
        'ni
        '
        Me.ni.Icon = CType(resources.GetObject("ni.Icon"), System.Drawing.Icon)
        Me.ni.Text = "ni"
        Me.ni.Visible = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.Black
        Me.BackgroundImage = CType(resources.GetObject("$this.BackgroundImage"), System.Drawing.Image)
        Me.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None
        Me.ClientSize = New System.Drawing.Size(502, 445)
        Me.Controls.Add(Me.lblStatus)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.btnConnect)
        Me.Controls.Add(Me.lstPorts)
        Me.Controls.Add(Me.lstConsole)
        Me.ForeColor = System.Drawing.SystemColors.ControlLightLight
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Spotify - Arduino Remote"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents lstConsole As System.Windows.Forms.ListBox
    Friend WithEvents btnConnect As System.Windows.Forms.Button
    Friend WithEvents lstPorts As System.Windows.Forms.ComboBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents lblStatus As System.Windows.Forms.Label
    Friend WithEvents ni As System.Windows.Forms.NotifyIcon

End Class
