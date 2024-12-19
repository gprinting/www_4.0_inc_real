<?php
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/common/mail/src/Exception.php");
include_once(INC_PATH . "/com/nexmotion/common/mail/src/SMTP.php");
include_once(INC_PATH . "/com/nexmotion/common/mail/src/PHPMailer.php");
include_once(INC_PATH . "/com/nexmotion/common/mail/src/PHPMailer.php");

function sendMail($subject, $content, $mailto, $mailtoname) {

    $mail               = new PHPMailer\PHPMailer\PHPMailer();
    $head               = $subject;
    $body               = $content;
    $receiver           = $mailtoname;
    
    $mail->IsHTML(true);
    $mail->IsSMTP();
    $mail->SMTPSecure   = "tls";
    $mail->SMTPAuth   = true;
    $mail->AuthType = "LOGIN";
    $mail->Port         = 587;
    $mail->Host         = "smtp.gmail.com";
    //$mail->SMTPOptions = array ('ssl' => array('verify_peer' => false, 'verify_peer_name' => false, 'allow_self_signed' => true));

    
    // 굿프린팅 공식계정, 비밀번호
    $mail->Username     = "goodprintingweb@gmail.com";
    $mail->Password     = "svnzsdqwwtyjlzuu";

    $mail->SMTPKeepAlive = true;
    //$mail->SMTPDebug = 1;

    $mail->SetFrom($mail->Username, "=?UTF-8?B?" .base64_encode("굿프린팅") . "?=");
    $mail->AddAddress($mailto, $receiver);
    
    $mail->Subject      = "=?UTF-8?B?" . base64_encode($head) . "?=";
    $mail->Body         = $body;
    
    if (!$mail->Send()) {
        echo "메일 전송에 실패 하였습니다.\n\n". $mail->ErrorInfo;
    } else {
        echo "1";
    }

}
?>
