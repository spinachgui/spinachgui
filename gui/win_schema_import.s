    .section .rodata
    .global _spinxmlSchema
    .align  4
_spinxmlSchema:
    .incbin "data/spinxml_schema.xsd"
    .int    0
    .global _spinxmlSchemaSize
    .align  4
_spinxmlSchemaSize:
    .int    _spinxmlSchemaSize - _spinxmlSchema
