#include "PsaFile.h"
#include "debug.h"

PsaFile::PsaFile()
{
    this->big_endian = false;
}

PsaFile::~PsaFile()
{

}

void PsaFile::Reset()
{
    attributes.clear();
}

bool PsaFile::Load(const uint8_t *buf, size_t size)
{
    Reset();

    if (!buf || size < sizeof(PSAHeader))
        return false;

    const PSAHeader *hdr = (const PSAHeader *)buf;
    if (hdr->signature != PSA_SIGNATURE)
        return false;

    attributes.resize(hdr->num_attributes+1);
    const PSAAttributes *file_attrs = (const PSAAttributes *)GetOffsetPtr(hdr, hdr->header_size+(uint32_t)attributes.size()*8, true);

    for (size_t i = 0; i < attributes.size(); i++)
    {
        PsaAttributes &attr = attributes[i];

        attr.hea = file_attrs[i].hea;
        attr.ki = file_attrs[i].ki;
        attr.sta = file_attrs[i].sta;
        attr.atk = file_attrs[i].atk;
        attr.str = file_attrs[i].str;
        attr.bla = file_attrs[i].bla;
    }

    return true;
}

float PsaFile::GetHea(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].hea;
}

float PsaFile::GetKi(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].ki;
}

float PsaFile::GetSta(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].sta;
}

float PsaFile::GetAtk(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].atk;
}

float PsaFile::GetStr(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].str;
}

float PsaFile::GetBla(uint16_t points, bool *error) const
{
    if (points >= attributes.size())
    {
        if (error)
            *error = true;

        return 0.0f;
    }

    if (error)
        *error = false;

    return attributes[points].bla;
}




