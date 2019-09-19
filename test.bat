macroGetFullFileName()
{
    hbuf = GetCurrentBuf()
    hbufClip = GetBufHandle("Clipboard")
    EmptyBuf(hbufClip)
    AppendBufLine(hbufClip,GetBufName(hbuf))
}
