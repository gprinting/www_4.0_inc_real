<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 5:58
 * desc: 접수중
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class ReceiptingState extends AbstractOrderByCountState
{
    public function ToNextState()
    {
        parent::ToNextState();
    }
}

?>