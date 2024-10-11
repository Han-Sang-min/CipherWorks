namespace workhardd
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }

            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.InputBox = new System.Windows.Forms.TextBox();
            this.Send_BT = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // InputBox
            // 
            this.InputBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.14286F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InputBox.Location = new System.Drawing.Point(88, 627);
            this.InputBox.Name = "InputBox";
            this.InputBox.Size = new System.Drawing.Size(782, 45);
            this.InputBox.TabIndex = 0;
            this.InputBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            // 
            // Send_BT
            // 
            this.Send_BT.AutoSize = true;
            this.Send_BT.Location = new System.Drawing.Point(884, 616);
            this.Send_BT.Name = "Send_BT";
            this.Send_BT.Size = new System.Drawing.Size(100, 45);
            this.Send_BT.TabIndex = 1;
            this.Send_BT.Text = "Send";
            this.Send_BT.UseVisualStyleBackColor = true;
            this.Send_BT.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.85714F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(88, 292);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(896, 66);
            this.label1.TabIndex = 2;
            this.label1.Text = "서버 반환 대기중";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // Form1
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1068, 704);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Send_BT);
            this.Controls.Add(this.InputBox);
            this.Location = new System.Drawing.Point(15, 15);
            this.Name = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private System.Windows.Forms.Label label1;

        private System.Windows.Forms.TextBox InputBox;
        private System.Windows.Forms.Button Send_BT;

        #endregion
    }
}