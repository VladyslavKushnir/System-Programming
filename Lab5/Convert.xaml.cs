using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace Lab5
{
    public sealed partial class Convert : Page
    {
        public Convert()
        {
            this.InitializeComponent();
            Operations_SelectionChanged(null, null);
        }

        private void HyperlinkButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }

        private void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            int a;

            Result1.Text = "";
            Result2.Text = "";
            Result3.Text = "";
            Result4.Text = "";

            if (!Int32.TryParse(A.Text, out a))
            {
                Result1.Text = "Ви ввели не число!";
                return;
            }

            switch (Operations.SelectedIndex)
            {
                case 0:
                    Result1.Text = (a / 3.281).ToString();
                    Result2.Text = (a * 39.37).ToString();
                    Result3.Text = (a * 1.094).ToString();
                    Result4.Text = (a / (double)1609).ToString();
                    break;
                case 1:
                    Result1.Text = (a * 2.205).ToString();
                    Result2.Text = (a * 35.274).ToString();
                    Result3.Text = (a * 0.01).ToString();
                    Result4.Text = (a * 0.001).ToString();
                    break;
                case 2:
                    Result1.Text = (a * 2.103).ToString();
                    Result2.Text = (a / 3.785).ToString();
                    Result3.Text = (a * 0.01).ToString();
                    Result4.Text = (a * 0.001).ToString();
                    break;
            }
        }

        private void Operations_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ResultText1 != null & ResultText2 != null & ResultText3 != null & ResultText4 != null)
            {
                Result1.Text = "";
                Result2.Text = "";
                Result3.Text = "";
                Result4.Text = "";

                switch (Operations.SelectedIndex)
                {
                    case 0:
                        Input.Text = "Метри";
                        ResultText1.Text = "Фут";
                        ResultText2.Text = "Дюйм";
                        ResultText3.Text = "Ярд";
                        ResultText4.Text = "Милі";
                        break;
                    case 1:
                        Input.Text = "Кілограми";
                        ResultText1.Text = "Фунти";
                        ResultText2.Text = "Унції";
                        ResultText3.Text = "Центнери";
                        ResultText4.Text = "Тони";
                        break;
                    case 2:
                        Input.Text = "Літри";
                        ResultText1.Text = "Пінти";
                        ResultText2.Text = "Галони";
                        ResultText3.Text = "куб/см";
                        ResultText4.Text = "куб/м";
                        break;
                }
            }
        }


    }
}
