<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 5:34
 * desc : 접수대기인 주문의 상태변경 (1320)
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class ReceiptWatingState extends AbstractOrderByCountState
{
    public function ToNextState()
    {
        parent::ToNextState();
    }
}


?>















