#include "Material.h"

Material::Material(float *ambient, float *diffuse, float *specular, float shininess)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{
}

Material::Material(float *ambient, unsigned int diffuse, unsigned int specular, float shininess)
    : m_ambient(ambient), m_material_diffuse(diffuse), m_material_specular(specular), m_shininess(shininess)
{
}

float *Material::GetAmbient()
{
    return m_ambient;
}

float *Material::GetDiffuse()
{
    return m_diffuse;
}

float *Material::GetSpecular()
{
    return m_specular;
}

float Material::GetShininess()
{
    return m_shininess;
}

unsigned int Material::GetMaterialDiffuse()
{
    return m_material_diffuse;
}

unsigned int Material::GetMaterialSpecular()
{
    return m_material_specular;
}
